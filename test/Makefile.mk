#
# Copyright (C) 2014. See LICENSE in top-level directory.
#

check_PROGRAMS += test/test_assert \
                  test/test_integer_passthrough \
                  test/test_float_sum \
                  # end

TESTS += test/test_assert \
         test/test_integer_passthrough \
         test/test_float_sum \
         # end

XFAIL_TESTS += test/test_assert \
               # end

test_test_assert_LDADD = libkahanmpi.la
test_test_integer_passthrough_LDADD = libkahanmpi.la
test_test_float_sum_LDADD = libkahanmpi.la
