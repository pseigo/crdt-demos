# CRDT Client (C++)

> **Note**: This is a work in progress!

## How to Run It

Install [CMake](https://cmake.org/download/) and a C++ compiler.

```sh
# (Assuming you are in the `client-cpp` directory.)

rm -rf build # (Deletes a previous build)
mkdir build # (The directory name is arbitrary)
cd build

# Configure and build
cmake -S .. -B .

# Run
./src/CrdtClientCpp
```

- For help with installing a C++ compiler or CMake, please search for instructions specific to your operating system distribution. Some popular compilers are [Clang](https://clang.llvm.org), [GCC](https://gcc.gnu.org), and [MSVC](https://learn.microsoft.com/en-us/cpp/build/reference/c-cpp-building-reference).
- For help with CMake, please reference the [CMake documentation](https://cmake.org/cmake/help/latest/index.html).

## Demo

Up to date as of commit `3ac73bfcb3afcb682fe4ba640a009ac72c5a8a26` (2023-09-22). Comments have been added.

### Two Nodes

```log
// Node 1 creates a StoreItem and broadcasts it
[info][node_1] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 7.990000, "currency": "cad"}`
[info] Broadcasting to 'crdt:store_items': {"source": "node_1", "timestamp": [{"node_id": "node_1", "count": 1}, {"node_id": "node_2", "count": 0}], "data": {"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 7.990000, "currency": "cad"}}

// Node 2 receives broadcast
[info][node_2] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 7.990000, "currency": "cad"}`

// Node 2 doubles the price and broadcasts it
[info] Broadcasting to 'crdt:store_items': {"source": "node_2", "timestamp": [{"node_id": "node_1", "count": 1}, {"node_id": "node_2", "count": 2}], "data": {"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 15.980000, "currency": "cad"}}

// Node 2 receives its own broadcast
[info][node_2] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 15.980000, "currency": "cad"}`

// Node 1 receives broadcast
[info][node_1] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 15.980000, "currency": "cad"}`

// Node 1 doubles the price, changes the name, and broadcasts it
[info] Broadcasting to 'crdt:store_items': {"source": "node_1", "timestamp": [{"node_id": "node_1", "count": 3}, {"node_id": "node_2", "count": 2}], "data": {"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Nanofiber Cloth", "category": "cleaning", "price": 31.960000, "currency": "cad"}}

// Node 1 receives its own broadcast
[info][node_1] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Nanofiber Cloth", "category": "cleaning", "price": 31.960000, "currency": "cad"}`

// Node 2 receives broadcast
[info][node_2] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Nanofiber Cloth", "category": "cleaning", "price": 31.960000, "currency": "cad"}`
```

### Three Nodes

```log
// Node 1 creates a StoreItem and broadcasts it
[info][node_1] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 7.990000, "currency": "cad"}`
[info] Broadcasting to 'crdt:store_items': {"source": "node_1", "timestamp": [{"node_id": "node_1", "count": 1}, {"node_id": "node_2", "count": 0}, {"node_id": "node_3", "count": 0}], "data": {"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 7.990000, "currency": "cad"}}

// Nodes 2 and 3 receive broadcast
[info][node_2] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 7.990000, "currency": "cad"}`
[info][node_3] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 7.990000, "currency": "cad"}`

// Node 2 doubles the price and broadcasts it
[info] Broadcasting to 'crdt:store_items': {"source": "node_2", "timestamp": [{"node_id": "node_1", "count": 1}, {"node_id": "node_2", "count": 2}, {"node_id": "node_3", "count": 0}], "data": {"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 15.980000, "currency": "cad"}}

// Node 2 receives its own broadcast
[info][node_2] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 15.980000, "currency": "cad"}`

// Node 1 receives broadcast
[info][node_1] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Microfiber Cloth", "category": "cleaning", "price": 15.980000, "currency": "cad"}`

// Node 1 doubles the price, changes the name, and broadcasts it
[info] Broadcasting to 'crdt:store_items': {"source": "node_1", "timestamp": [{"node_id": "node_1", "count": 3}, {"node_id": "node_2", "count": 2}, {"node_id": "node_3", "count": 0}], "data": {"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Nanofiber Cloth", "category": "cleaning", "price": 31.960000, "currency": "cad"}}

// Node 1 receives its own broadcast
[info][node_1] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Nanofiber Cloth", "category": "cleaning", "price": 31.960000, "currency": "cad"}`

// Nodes 2 and 3 receive broadcast
[info][node_2] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Nanofiber Cloth", "category": "cleaning", "price": 31.960000, "currency": "cad"}`
[info][node_3] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Nanofiber Cloth", "category": "cleaning", "price": 31.960000, "currency": "cad"}`

// Node 3 multiplies the price 10x, changes the name, and broadcasts it
[info] Broadcasting to 'crdt:store_items': {"source": "node_3", "timestamp": [{"node_id": "node_1", "count": 3}, {"node_id": "node_2", "count": 2}, {"node_id": "node_3", "count": 4}], "data": {"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Supernanofiber Cloth", "category": "cleaning", "price": 319.600000, "currency": "cad"}}

// Node 3 receives its own broadcast
[info][node_3] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Supernanofiber Cloth", "category": "cleaning", "price": 319.600000, "currency": "cad"}`

// Nodes 1 and 2 receive broadcast
[info][node_1] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Supernanofiber Cloth", "category": "cleaning", "price": 319.600000, "currency": "cad"}`
[info][node_2] current value of '987C30DB-A335-40F0-8694-0921EAB9406F' is: `{"id": "987C30DB-A335-40F0-8694-0921EAB9406F", "name": "Supernanofiber Cloth", "category": "cleaning", "price": 319.600000, "currency": "cad"}`
```

## License

Copyright (c) 2023 Peyton Seigo.

- Otherwise, code belonging to other copyright holders should be cited in the same file.
- Third party dependencies are in the `third_party` directory and their licenses are in the `licenses` directory.

