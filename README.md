# Network Topology Based Influence Maximization

## Overview

This project identifies **top-k influential nodes** in social networks using graph topology, community structure, and centrality measures.

The algorithm is tested on multiple real-world datasets such as:

* Email network
* Facebook network
* Karate club network
* Last.fm network

---

## Project Structure

├── finalcode.c                # Main implementation
│
├── Email.txt                 # Email dataset
├── Email_comm.txt            # Community data (Email)
│
├── facebook.txt              # Facebook dataset
├── facebook_comm.txt         # Community data (Facebook)
│
├── karate3.txt               # Karate dataset
├── karate_comm.txt           # Community data (Karate)
│
├── lastfm.txt                # Last.fm dataset
├── lastfm_comm.txt           # Community data (Last.fm)
│
├── result_Email.txt          # Output (Email)
├── result_facebook.txt       # Output (Facebook)
├── result_karate3.txt        # Output (Karate)
├── result_lastfm.txt         # Output (Last.fm)
│
├── figures.docx              # Graphs / analysis
└── README.md

---

## How the Program Works

1. Reads a network dataset (`*.txt`)

2. Uses corresponding community file (`*_comm.txt`)

3. Builds graph using CSR representation

4. Computes:

   * Boundary Score (BS)
   * Closeness Centrality (CC)

5. Calculates:

   CS(v) = BS(v) + CC(v)

6. Selects **Top-K influential nodes**

7. Writes results to `result_*.txt`

---

##  How to Run

### 1. Clone the repository

```id="a1"
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name
```

### 2. Compile the code

```id="a2"
gcc finalcode.c -o finalcode
```

### 3. Run the program

```id="a3"
./finalcode
```

---

## Input Files

The program uses these input files:

* Network file → `Email.txt`, `facebook.txt`, `karate3.txt`, `lastfm.txt`
* Community file → `*_comm.txt`

Make sure all `.txt` files are in the same directory as the executable.

---

## Output Files

After execution, results are saved in:

* `result_Email.txt`
* `result_facebook.txt`
* `result_karate3.txt`
* `result_lastfm.txt`

Each file contains:

* Centrality values
* Boundary scores
* Composite scores
* Top-K influential nodes

---

## Technologies Used

* C Programming
* Graph Algorithms
* BFS (Breadth First Search)

---

## Applications

* Social Network Analysis
* Influence Maximization
* Viral Marketing

---

## 👩‍💻 Authors

* Kaberi Bera

Supervisor:

* Dr. Arnab Kumar Ghoshal
   ,Assistant Professor
  Department of Computer Science, Asutosh College
  
Project Partners:

* Arpita Bhaumik
* Md Fradeen Alam


## Note

* Program assumes **unweighted networks**
* File names may need to match exactly as used in code
* Works in Linux / Windows (MinGW)

---
