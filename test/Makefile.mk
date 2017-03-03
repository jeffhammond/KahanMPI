#
# Copyright (C) 2014. See LICENSE in top-level directory.
#

check_PROGRAMS += test/test_assert \
		  test/test_integer_passthrough \
                  # end

TESTS += test/test_assert \
         test/test_integer_passthrough \
         # end

XFAIL_TESTS += test/test_assert \
               # end

test_test_assert_LDADD = libkahanmpi.la
