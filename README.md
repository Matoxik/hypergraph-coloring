## Hypergraph Coloring

A C++ implementation for generating hypergraphs, converting them to graphs, and comparing three graph coloring algorithms.

### Features
- **Hypergraph Generation**
  - Random hypergraph generator with configurable vertex/hyperedge counts
  - JSON serialization/deserialization using `nlohmann/json`
  
- **Graph Conversion**
  - Transforms hyperedges into adjacency matrix representation
  - Converts hyperedges to complete subgraphs (cliques)

- **Coloring Algorithms**
  -  **Greedy Coloring**: Sequential vertex coloring
  -  **Randomized Greedy**: Coloring with shuffled vertex order
  -  **Brute Force**: Exact solution using backtracking (for small graphs)

### Example
- **For input data:**
  
![obraz](https://github.com/user-attachments/assets/0a66c42f-fe95-4388-a0a9-24a7f3f26f13)

- **Calculated solution using 3 algorithms**

![obraz](https://github.com/user-attachments/assets/34ae6c9f-058d-497a-b9d7-0f6aa82e9974)

*Every number defines different color*

- **Visualization**

![colorgraf](https://github.com/user-attachments/assets/75262f79-ca8e-4403-aa45-de8c99329a1c)

