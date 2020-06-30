#pragma once


struct Node {
    Node const* up = nullptr;
    Node const* down = nullptr;
    Node const* left = nullptr;
    Node const* right = nullptr;
    const bool end = true;
};