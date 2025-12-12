🧠 Dynamic Memory Management Visualizer
A C-based console application that simulates and visualizes operating‑system–level memory management: paging, segmentation, virtual memory, and multiple page‑replacement policies (FIFO, LRU, Optimal). It is designed as an educational, experiment‑friendly toolkit to help students and engineers understand how real OS memory managers behave under different workloads.

✨ Highlights
🔍 End‑to‑end memory life‑cycle: from process creation and logical address generation to frame allocation, page faults, and replacement.

📐 Multiple memory models: paging, segmentation, and combined virtual‑memory semantics with clear address translation steps.

♻️ Configurable replacement policies: FIFO, LRU, Optimal, switchable at runtime to compare hit/fault behavior.

📊 Real‑time analytics: live statistics on hits, faults, hit ratio, fault ratio, and memory usage per process.

🧑‍💻 Interactive console UI: menu‑driven interface with colored output, step‑by‑step demos, and a ready‑to‑run sample scenario.

🛠️ Scripted toolchain: build, warning‑fix, and demo scripts to quickly compile and run consistent experiments.

🧱 Module-Level Architecture
This is how the project is decomposed into logical components and files.

🧑‍💻 1. User Interaction Layer
main.c

Central controller that exposes the main menu and drives the entire simulation loop.

Functions exposed via menu:

Initialize memory system

Create processes

Allocate / deallocate pages

Access memory (trigger hits and page faults)

Paging, Segmentation, Virtual‑Memory submenus

Page‑replacement algorithm selection (FIFO / LRU / Optimal)

Statistics and memory‑layout visualization

Demo simulation (pre‑configured workloads)

run_demo.sh

Orchestrates a curated demonstration using typical processes (e.g., “Browser”, “Text Editor”) and randomized access patterns.

💾 2. Core Simulation Engine
common_defs.h

Defines global simulation limits and core data models:

MemoryFrame – physical frame metadata (frame id, mapped page, owning process, timestamps, reference/dirty bits, free flag).

Process – process descriptor with page/segment tables, memory usage, and per‑process fault counters.

PageTableEntry, SegmentTableEntry – logical mapping structures.

Statistics – global counters for hits, faults, and derived metrics (ratios, average access time).

memory_manager.c / memory_manager.h

Acts as the policy‑aware memory orchestrator:

Initializes physical frames and per‑process tables.

Creates and configures processes.

Allocates and deallocates pages on demand.

Performs simulated memory accesses (access_memory) and computes physical addresses.

Detects page faults and triggers page replacement if no free frame is available.

Tracks current_time, updates access timestamps, and maintains global statistics.

Delegates victim selection to FIFO/LRU/Optimal via a configurable algorithm_type.

📐 3. Address Semantics & Algorithms
paging.c / paging.h

Implements the paging pipeline:

Paging system initialization (page size, number of pages, VA space).

Logical‑to‑physical translation based on page tables.

Page table visualization.

Page‑fault simulation and handling walkthroughs.

segmentation.c / segmentation.h

Implements the segmentation engine:

Segment descriptor creation (base, limit, protection).

Segment table display.

Segment‑based logical address translation and bounds checking.

virtual_memory.c / virtual_memory.h

Models virtual memory behavior:

Demand paging over a larger virtual space.

Interaction between access patterns, page faults, and backing‑store behavior (conceptually).

fifo.c / fifo.h, lru.c / lru.h, optimal.c / optimal.h

Implement the page‑replacement policy suite:

FIFO – queue‑based victim selection.

LRU – time‑based recency tracking using access timestamps.

Optimal – look‑ahead policy using a generated reference string (theoretical upper bound).

📊 4. Visualization, Logging & Tooling
console_gui.c / console_gui.h

High‑level console UI helpers: menus, prompts, validation messages, success/error banners, and colored feedback.

memory_display.c / memory_display.h

Visualizes memory state:

Frame‑by‑frame memory grid (which frame holds which page/process).

Page table and segment table views.

Compact “snapshot” view of current allocation and usage.

logger.c / logger.h + memory_simulation.log

Structured logging of simulation events to a log file, including start/stop messages, major operations, and diagnostic messages for later analysis.

Tooling scripts (compile.sh, compile_correct.sh, fix_warnings.sh, fix_all_includes.sh, final_fix.sh)

Provide a repeatable build pipeline and helper routines to maintain a clean, warning‑free build.

Misc metadata (notes.h, `last_updated
