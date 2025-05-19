#pragma once

struct Request {
    int id;
    int pickupNodeId;
    int deliveryNodeId;

    bool operator==(const Request& other) const {
        return id == other.id &&
            pickupNodeId == other.pickupNodeId &&
            deliveryNodeId == other.deliveryNodeId;
    }
};