/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include "ring_buf.h"
#include <zephyr/ztest.h>

#include <errno.h>

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void* f)
{
    ARG_UNUSED(f);
    rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
    zassert_true(rb_is_empty(), "Fresh buffer must be empty");
    zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
    // precondition
    zassume_ok(rb_push(99));

    // input
    zassert_ok(rb_init(4));

    // expected
    zassert_true(rb_is_empty(), "Fresh buffer must be empty");
    zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_init_with_zero_capacity_returns_error)
{
    // precondition -> none

    // input & expected result
    zassert_equal(rb_init(0), -EINVAL);
}

ZTEST(ring_buf_init, test_init_with_excessive_capacity_returns_error)
{
    // precondition -> none

    // input & expected result
    zassert_equal(rb_init(RING_BUF_MAX_CAPACITY + 1), -EINVAL);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
    int popped_value = -1;

    // precondition -> none

    // input
    zassert_ok(rb_push(42));
    zassert_ok(rb_pop(&popped_value));

    // expected
    zassert_equal(popped_value, 42);
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
    int popped_value  = -1;
    int popped_value2 = -1;
    int popped_value3 = -1;

    // precondition -> none

    // input
    zassert_ok(rb_push(1));
    zassert_ok(rb_push(2));
    zassert_ok(rb_push(3));

    zassert_ok(rb_pop(&popped_value));
    zassert_ok(rb_pop(&popped_value2));
    zassert_ok(rb_pop(&popped_value3));

    // expected
    zassert_equal(popped_value, 1);
    zassert_equal(popped_value2, 2);
    zassert_equal(popped_value3, 3);
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
    // precondition -> fill the buffer to capacity
    zassume_ok(rb_push(1));
    zassume_ok(rb_push(2));
    zassume_ok(rb_push(3));
    zassume_ok(rb_push(4));

    // input -> push one more value and expected value
    zassert_equal(rb_push(5), -ENOSPC);
}

ZTEST(ring_buf_push_pop, test_pop_on_empty_buf_returns_error)
{
    int popped_value = -1;
    // precondition -> empty buffer

    // input -> pop from empty buffer and expected value
    zassert_equal(rb_pop(&popped_value), -ENODATA);
    zassert_equal(popped_value, -1);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
    int peeked_value1 = -1;
    int peeked_value2 = -1;

    // precondition -> buffer has one element
    zassume_ok(rb_push(7));

    // input
    zassert_ok(rb_peek(&peeked_value1));
    zassert_ok(rb_peek(&peeked_value2));

    // expected
    zassert_equal(peeked_value1, 7);
    zassert_equal(peeked_value2, 7);
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
    // precondition -> none

    // input -> pop with NULL pointer
    zassert_equal(rb_pop(NULL), -EINVAL);
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
    // precondition -> buffer is full
    zassume_ok(rb_push(1));
    zassume_ok(rb_push(2));
    zassume_ok(rb_push(3));
    zassume_ok(rb_push(4));

    // input & expected
    zassert_true(rb_is_full());
}

ZTEST(ring_buf_boundaries, test_peek_with_null_ptr_returns_error)
{
    // precondition -> none

    // input & expected
    zassert_equal(rb_peek(NULL), -EINVAL);
}

ZTEST(ring_buf_boundaries, test_peek_with_empty_buf_returns_error)
{
    int peeked_value = -1;
    // precondition -> none

    // input & expected
    zassert_equal(rb_peek(&peeked_value), -ENODATA);
	zassert_equal(peeked_value, -1);
}