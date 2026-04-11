# SQL Query Optimization Engine (C++)

A SQL query optimization engine built from scratch in C++. This system simulates the query compilation phase of a relational database, transforming raw SQL queries into highly efficient execution plans using both **Heuristic-based Rules** and **Cost-Based Optimization (CBO)**.

## Key Features

Our engine implements a multi-stage optimization pipeline to minimize query execution time and resource consumption:

  * **Heuristic Optimization:**
      * **Selection Pushdown:** Filters are moved closer to the data source to reduce row cardinality early.
      * **Projection Pushdown:** Unnecessary columns are pruned early to minimize tuple width.
      * **Limit Pushdown:** Constraints are propagated down the tree to stop data scanning as soon as the limit is met.
  * **Cost-Based Optimization (CBO):**
      * **Join Reordering:** Dynamically swaps join operands based on table cardinality (Smallest Table First heuristic).
      * **Cost Estimation:** A recursive cost model that evaluates plan efficiency based on row counts and operation complexity.
  * **Tree Visualization:** Generates human-readable ASCII representations of the Query Plan before and after optimization.

-----

## System Architecture

The project is modularized into four distinct layers:

1.  **Parser:** Converts SQL strings into a structured Query object.
2.  **Planner:** Generates the initial Logical Query Plan (represented as a recursive Tree/AST).
3.  **Optimizer:** The "Brain" of the system. It applies transformation rules to rewrite the tree.
4.  **Cost Estimator:** Analyzes the tree and assigns a numerical cost based on simulated table statistics.

-----

## Performance Impact

In our benchmarking tests, the engine achieved significant efficiency gains by reducing the search space and intermediate result sizes:

| Optimization Level | Estimated Cost | Improvement |
| :--- | :--- | :--- |
| **Unoptimized Plan** | 505,000.00 | 0% |
| **Optimized Plan** | 101,000.00 | **80%** |

-----

## Installation & Usage

### Prerequisites

  * A C++ compiler (GCC/G++ 7.0 or higher)
  * Make (optional)

### Compilation

Navigate to the `src` directory and run:

```bash
g++ main.cpp cost/*.cpp optimizer/*.cpp planner/*.cpp -o query_engine
```

### Running the Engine

```bash
./query_engine
```

-----

## 📂 Project Structure

```text
├── src
│   ├── main.cpp            # Entry point & Demo runner
│   ├── planner
│   │   ├── Node.h
│   │   └── QueryPlan.h
|   |   |__ QueryPlan.cpp    # Logical Plan builder
|   |
│   ├── optimizer
│   │   ├── Optimiser.h     # Transformation rules
│   │   └── Optimiser.cpp   # Implementation of Pushdowns & Reordering
│   └── cost
│   |    ├── CostEstimator.h # Cost model definitions
│   |    └── CostEstimator.cpp # Size and Cost logic
|   └── parser
│   |    ├── parser.h
│   |   └── parser.cpp
|   |
|   |__visualisation
|   |
|   |__ output
|       |__ before.dot
|       |__ after.dot
|
└── README.md
|__.gitignore
```

-----


## Screenshots
to be added once the project is completed : )
