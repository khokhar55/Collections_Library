# Daily Design Journal — Day 1

## Section 1 — Specific Bug
*(Paste the exact compiler error message or runtime output here)*
No bugs yet. Setting up project structure and drafting Phase 0 design proposals.

## Section 2 — Failed Attempt
*(Describe what you tried first and why it failed)*
N/A for Phase 0.

## Section 3 — Memory Diagram
*(Hand-drawn memory diagram, photographed and submitted every day)*
[Placeholder for photograph: e.g., memory_diagram_day1.jpg]

## Section 4 — Code Reference
*(Commit hash, filename, and relevant line numbers)*
Commit: *(To be added)*
Files: `docs/dynamic_array.md`, `docs/linked_list.md`

## Section 5 — Learning Reflection
*(What understanding changed this day?)*
Today was focused on design rather than implementation. The biggest takeaway was understanding the exact structural requirements for the collections, particularly how the `LinkedList` needs to be implemented as a *singly linked list* to properly serve as the collision chain backend for the future `HashMap`, and why that specific design choice makes `deleteBack()` an O(n) operation while keeping memory usage and code complexity minimal.
