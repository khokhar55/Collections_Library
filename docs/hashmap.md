# Design Proposal: HashMap&lt;K, V&gt;

> **What is it?** A key-value storage system that uses a mathematical hash function to convert any key into an exact array index, providing near-instant O(1) lookups on average. Collisions — when two different keys land on the same index — are handled by building a small linked chain at that bucket.

---

## Section 1 — Public API

### Lifecycle Management (Rule of Three)

| Member | Purpose |
|---|---|
| `HashMap()` | Default constructor — 16 buckets, load threshold 0.7 |
| `~HashMap()` | Walks every bucket chain and deletes every Entry node, then frees the bucket array |
| `HashMap(const HashMap&)` | Deep copy — new bucket array, all chains duplicated |
| `operator=(const HashMap&)` | Deep copy assignment — purge self, deep copy source |

> **Why Rule of Three instead of Five?** HashMap does not benefit from move semantics as naturally as DynamicArray because its internal bucket array is a pointer-to-pointers; move is possible but Rule of Three covers all correctness requirements for Phase 0.

### Core Operations

| Function | Signature | Description |
|---|---|---|
| `set` | `void set(std::string key, std::string value)` | Insert or overwrite. Triggers rehash if load factor exceeds threshold |
| `get` | `std::string get(std::string key) const` | Returns value for key, or `""` (empty string) if key not found |
| `exists` | `bool exists(std::string key) const` | Returns `true` if the key is present |
| `remove` | `void remove(std::string key)` | Unlinks and deletes the matching Entry node from its chain |
| `clear` | `void clear()` | Deletes all Entry nodes across all chains; resets count to 0 |

### Sizing & Metrics

| Function | Signature | Description |
|---|---|---|
| `size` | `int size() const` | Returns number of stored key-value pairs (not bucket count) |
| `bucketCount` | `int bucketCount() const` | Returns current number of buckets |
| `loadFactor` | `float loadFactor() const` | Returns `numEntries / numBuckets` |

### Debug Utility

| Function | Signature | Description |
|---|---|---|
| `print` | `void print() const` | Dumps all buckets and chains to stdout for debugging |

---

## Section 2 — Internal Structure

### Entry Node

Each stored key-value pair is wrapped in an `Entry` node:

```
Entry {
    key    : std::string   ← the key
    value  : std::string   ← the stored value
    next   : Entry*        ← pointer to next entry in the same bucket chain
}
```

### Bucket Array

The HashMap owns a heap-allocated array of `Entry*` pointers. Each pointer is either `nullptr` (empty bucket) or the head of a chain of `Entry` nodes:

```
buckets[0] → nullptr
buckets[1] → Entry{"name","Arun"} → Entry{"city","Chd"} → nullptr
buckets[2] → nullptr
...
buckets[5] → Entry{"age","25"} → nullptr
```

---

## Section 3 — Memory Layout

### Full Structure Diagram

```mermaid
graph TD
    subgraph Stack["🔵 STACK FRAME"]
        direction TB
        BP["buckets : Entry**"]
        NB["numBuckets : int = 8"]
        NE["numEntries : int = 3"]
        LT["loadThreshold : float = 0.7"]
    end

    subgraph BktArr["🟢 HEAP — Bucket Array (contiguous Entry* pointers)"]
        direction LR
        b0["[0] null"]
        b1["[1] ●"]
        b2["[2] null"]
        b3["[3] null"]
        b4["[4] null"]
        b5["[5] ●"]
        b6["[6] null"]
        b7["[7] null"]
    end

    subgraph E1["🟠 HEAP — Entry"]
        direction TB
        k1["key = name"]
        v1["value = Arun"]
        n1["next ──>"]
    end

    subgraph E2["🔴 HEAP — Entry (collision)"]
        direction TB
        k2["key = city"]
        v2["value = Chd"]
        n2["next = null"]
    end

    subgraph E3["🟠 HEAP — Entry"]
        direction TB
        k3["key = age"]
        v3["value = 25"]
        n3["next = null"]
    end

    BP -->|"owns"| b0
    b1 -->|"chain head"| k1
    n1 -->|"collision"| k2
    b5 -->|"chain head"| k3

    style Stack fill:#1e3a5f,stroke:#4a9eff,color:#e0f0ff
    style BktArr fill:#1a3d2e,stroke:#51cf66,color:#d4f5e0
    style E1 fill:#78350f,stroke:#f59e0b,color:#fef3c7
    style E2 fill:#7f1d1d,stroke:#ef4444,color:#fecaca
    style E3 fill:#78350f,stroke:#f59e0b,color:#fef3c7
```

- **Stack (blue):** `buckets` pointer, `numBuckets`, `numEntries`, and `loadThreshold` live on the stack.
- **Heap — Bucket Array (green):** A contiguous array of `Entry*` pointers, all initialised to `nullptr`.
- **Heap — Entry nodes (orange):** Each `Entry` lives at an independent heap address.
- **Collision (red):** When "name" and "city" both hash to bucket [1], they form a chain. The red entry follows the orange one via `next`.

### Rehash Procedure

```mermaid
graph TD
    R1["① Allocate new bucket array\n(2 × numBuckets = 16 slots)\nall set to nullptr"]
    R2["② Walk every Entry in every\nchain of the OLD array"]
    R3["③ Re-hash each key against\nnew bucket count (16)\nnewIdx = hash(key) % 16"]
    R4["④ Prepend Entry to new array\nat newIdx — O(1)"]
    R5["⑤ Free the OLD bucket array\n(not the entries — they moved)"]
    R6["⑥ numBuckets = 16"]

    R1 --> R2 --> R3 --> R4 --> R5 --> R6

    style R1 fill:#78350f,stroke:#f59e0b,color:#fef3c7
    style R2 fill:#1e3a5f,stroke:#4a9eff,color:#e0f0ff
    style R3 fill:#1e3a5f,stroke:#4a9eff,color:#e0f0ff
    style R4 fill:#1e3a5f,stroke:#4a9eff,color:#e0f0ff
    style R5 fill:#7f1d1d,stroke:#ef4444,color:#fecaca
    style R6 fill:#1a3d2e,stroke:#51cf66,color:#d4f5e0
```

> **Critical:** Entries cannot be moved to the same bucket index after a rehash. `key % oldSize ≠ key % newSize` in general. Every entry must be **re-hashed** against the new bucket count.

---

## Section 4 — Complexity Estimates

| Operation | Best | Average | Worst | Explanation |
|---|---|---|---|---|
| `set(key, value)` | O(1) | **O(1) amortized** | O(n) | Hash gives bucket in O(1). Overwrite or prepend to chain. O(n) only when all n keys hash to the same bucket. Rehash is O(n) but rare — see amortized proof |
| `get(key)` | O(1) | O(1) | O(n) | Same logic: hash → bucket → walk chain. O(1) when chain is short |
| `exists(key)` | O(1) | O(1) | O(n) | Same as `get()` but returns bool |
| `remove(key)` | O(1) | O(1) | O(n) | Hash → bucket → find → unlink → delete |
| `rehash()` | O(n) | O(n) | O(n) | Must visit every stored entry and re-hash it — always O(n) |
| `size()` / `loadFactor()` | O(1) | O(1) | O(1) | Returns stored member variables |
| `clear()` | O(n) | O(n) | O(n) | Must delete every Entry node in every chain |
| Move constructor | O(1) | O(1) | O(1) | Steal `buckets` pointer, `numBuckets`, `numEntries` |
| Copy constructor | O(n) | O(n) | O(n) | Allocate new bucket array, `set()` every entry from source |

### Amortized O(1) for `set`

The same doubling argument as DynamicArray applies here:

- Rehash triggers when `loadFactor > 0.7`, i.e., when `numEntries > 0.7 × numBuckets`.
- After rehash, the table has 2× buckets and ≈ 0.35 load factor — far from the threshold.
- Between two consecutive rehashes at sizes B and 2B, there are ≈ 0.7B cheap O(1) insertions.
- Rehash cost is O(n). Spread across those 0.7B insertions → **≈ 1 extra op per insertion → O(1) amortized**.

```mermaid
graph LR
    subgraph Good["Load factor stays low ✅"]
        direction LR
        G1["Rehash at 0.7 × B\nB = 16 → 11 entries"]
        G2["New size = 32\nLoad resets to 0.35"]
        G3["~11 cheap inserts before next rehash"]
    end

    subgraph Bad["High load factor ❌"]
        direction LR
        B1["Threshold = 0.9 × B"]
        B2["Chains grow long"]
        B3["get() degrades toward O(n)"]
    end

    style Good fill:#1a3d2e,stroke:#51cf66,color:#d4f5e0
    style Bad fill:#7f1d1d,stroke:#ef4444,color:#fecaca
```

---

## Section 5 — Design Decisions

### Core Decisions

| Decision | Our Choice | What We Rejected | Reason |
|---|---|---|---|
| **Collision strategy** | Separate chaining (linked Entry nodes) | Linear probing (scan next slots) | Probing needs tombstone markers for deletion — complex and error-prone. Chaining makes deletion simple (pointer unlink). Chaining allows load factor > 1.0 and degrades gradually. |
| **Hash function** | djb2 (`hash = hash × 33 + c`) | ASCII character sum | ASCII sum gives the same hash for anagrams — "abc" and "cab" both sum to 294. djb2 is position-sensitive (multiply-and-add) so anagrams produce different hashes. |
| **Initial bucket count** | 16 | 8 (fills too fast), 32 (wasteful for small maps) | Power of 2. Holds 11 entries before 0.7 threshold triggers rehash. |
| **Load threshold** | 0.7 | 0.5 (rehash too often, wastes time), 0.9 (chains grow long, degrades lookup) | Industry standard — same value used by Java's `HashMap`. Balances chain length vs. rehash frequency. |
| **Rehash growth factor** | × 2 | Fixed step | Mirrors DynamicArray's doubling. Logarithmic rehash count over the object's lifetime → amortized O(1) per `set`. |
| **Missing key in `get()`** | Return `""` (empty string) | Throw exception | Missing key is a normal lookup result in Redis-style usage, not a programming error. Avoids mandatory try-catch at every call site. |

### Collision Strategies Compared

```mermaid
graph TD
    subgraph Chaining["Separate Chaining ✅ Our Choice"]
        C1["Load factor can exceed 1.0"]
        C2["Deletion = one pointer swap"]
        C3["Each bucket fully independent"]
        C4["Performance degrades gradually"]
    end

    subgraph Probing["Linear Probing ❌ Rejected"]
        P1["Cache-friendly (contiguous array)"]
        P2["Deletion requires tombstone markers"]
        P3["Performance cliff near load 1.0"]
        P4["Primary clustering worsens over time"]
    end

    style Chaining fill:#1a3d2e,stroke:#51cf66,color:#d4f5e0
    style Probing fill:#7f1d1d,stroke:#ef4444,color:#fecaca
```

### Hash Function Comparison

```mermaid
graph LR
    subgraph Naive["Naive ASCII Sum ❌"]
        N1["abc → 97+98+99 = 294"]
        N2["cab → 99+97+98 = 294"]
        N3["Same hash! → Collision!"]
    end

    subgraph DJB2["djb2 ✅"]
        D1["abc → 193,485,963"]
        D2["cab → 193,487,083"]
        D3["Different hashes — no collision"]
    end

    style Naive fill:#7f1d1d,stroke:#ef4444,color:#fecaca
    style DJB2 fill:#1a3d2e,stroke:#51cf66,color:#d4f5e0
```

**djb2 algorithm:**
```
hash = 5381
for each character c in key:
    hash = hash × 33 + c
return hash
```

The multiply-by-33 step makes the hash position-sensitive: character `c` at position `i` contributes differently from the same `c` at position `j`. This breaks the anagram collision problem of naive summation.

**Why not a cryptographic hash (SHA-256, MD5)?** Hash tables need *speed* and *even distribution*, not cryptographic collision resistance. djb2 runs in nanoseconds; SHA-256 is hundreds of times slower.

---

## Section 6 — C++ Tools Planned

| Tool | Header | Why We Need It |
|---|---|---|
| `new` / `delete` | built-in | Allocate individual `Entry` nodes and the `Entry*` bucket array |
| `std::string` comparison (`==`) | `<string>` | Compare keys during chain traversal for collision resolution |
| `std::out_of_range` | `<stdexcept>` | Optional: throw if `get()` is switched to exception-based error handling |
| `std::move` | `<utility>` | Move constructor and assignment (if implemented beyond Rule of Three) |
