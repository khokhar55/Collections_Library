# Build Log

> For every work session add an entry to your build log.

## Template

**Date:** [Date]
**Duration:** [Minutes]

**Goal:**
[What were you trying to achieve?]

**Problem Encountered:**
[What went wrong? Include any bugs or obstacles]

**What I Tried:**
[Steps taken to debug or fix the issue]

**Outcome:**
[What was the final result or realization?]

---

## Log Entries

**Date:** June 26
**Duration:** 30 minutes

**Goal:**
Set up the initial project structure, CMake build system, and draft the Phase 0 design proposals.

**Problem Encountered:**
N/A (Scaffolding phase)

**What I Tried:**
N/A

**Outcome:**
Successfully established the Git repository, CMake configuration, and finalized the Phase 0 design documents for DynamicArray and LinkedList.

---

**Date:** June 26
**Duration:** 15 minutes

**Goal:**
Implement Step 2 (Raw Memory & Teardown): Overloaded constructor `DynamicArray(int customCapacity)` and destructor.

**Problem Encountered:**
In the constructor, I forgot to assign the `customCapacity` parameter to the member variable `a_capacity`. Because `int` is a primitive type, `a_capacity` contained a garbage memory value (4219224) which was passed to `malloc`. This caused the test case to fail.

**What I Tried:**
I analyzed the compiler warning (`unused parameter 'customCapacity'`) and the test output (`Got: 4219224`). I documented this memory initialization bug in `day_1_journal.md`.

**Outcome:**
I added `a_capacity = customCapacity;` to the constructor. The tests successfully compiled and passed, proving the memory allocation now matches the requested capacity.

---

**Date:** June 26
**Duration:** 10 minutes

**Goal:**
Implement Step 3 (Element Access): Methods `get(index)` and `operator[](index)` to read and write to the array.

**Problem Encountered:**
No major bugs. I successfully implemented `std::out_of_range` bounds checking for `get()`, while keeping `operator[]` raw for maximum performance.

**What I Tried:**
Added a test case to explicitly check that calling `get(0)` on a newly constructed, empty array correctly throws an exception instead of returning uninitialized memory.

**Outcome:**
The test caught the exception exactly as designed, meaning our bounds checking logic is solid and safe.
