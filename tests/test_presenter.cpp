#include "test_headers.hpp"
#include "test_mock.hpp"

using ::testing::AtLeast;

TEST (Presenter, TransferCallbacks) {
    MockInterface interface;
    EXPECT_CALL(interface, add(1,2)).Times(AtLeast(1));
    interface.add(1,2);
}
