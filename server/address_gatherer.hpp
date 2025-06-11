#pragma once

#include "address.hpp"
#include "common_types.hpp"
class Presenter;
class AddressGatherer {
public:
    ~AddressGatherer();
    void set_presenter(std::shared_ptr<Presenter> presenter);
    void gather_local_address();
private:
    net::io_context context_;
    std::thread gather_thread_;
    std::weak_ptr<Presenter> presenter_;
};
