# Virtual Awaken 🤖

**Virtual Awaken** is a third-person action-adventure and platformer developed in **Unreal Engine 5**.  
The project follows the journey of **V-311**, an endearing laboratory robot navigating a desolate desert planet inhabited exclusively by machines.

---

# 🚀 Project Overview

The development of **Virtual Awaken** is structured into **four distinct phases**, each focused on expanding gameplay systems and environmental depth while preserving the core identity of the protagonist.

### Current Status
**Phase 1 – The Awakening** 🛠️ *(In Progress)*

This phase focuses on establishing the foundational gameplay pillars:

- **Navigation & Locomotion**  
  Movement systems designed to deliver a grounded and responsive robotic feel.

- **Environmental Logic**  
  Implementation of the core puzzle-solving mechanics and world navigation systems.

- **Basic Interaction**  
  Creation of the interaction layer between the protagonist and the world.

---

# 🗺️ Development Roadmap

The project evolves through incremental gameplay improvements across four phases.

| Phase | Focus | Status |
|------|------|------|
| **Phase 1** | Core exploration, basic mobility, and fundamental puzzles | 🟡 In Progress |
| **Phase 2** | Expansion of world interaction depth and logic-based challenges | 🔵 Planned |
| **Phase 3** | Increased environmental complexity and advanced navigation mechanics | 🔵 Planned |
| **Phase 4** | Final technical refinement, polish, and world-scale optimization | 🔵 Planned |

---

# 🛠️ Technical Implementation

This repository demonstrates a **professional, scalable architecture** built with a hybrid **C++ / Blueprint** workflow.

### Data-Driven Interaction System
A C++ framework that handles intelligent contextual interactions based on **view angle** and **distance detection**.

### Dynamic UI & Input
Full support for **real-time input switching (Keyboard / Gamepad)** using **Common UI**, ensuring a seamless player experience.

### Decoupled Actor Architecture
Universal use of **interfaces (`IInteractableInterface`)** to manage world objects such as:

- Doors  
- Switches  
- Platforms  

This prevents hard dependencies between gameplay actors.

### Smart UI Projection
Optimized **screen-space projection system** for interaction widgets with **automatic anchoring** to guarantee readability in all camera angles.

---

# ⚙️ Tech Stack

| Category | Technology |
|--------|--------|
| **Engine** | Unreal Engine 5.7 |
| **Language** | C++ |
| **Plugins** | Common UI / Common Input |

---

# 📌 Project Goals

- Build a **clean and scalable gameplay architecture**
- Showcase **professional Unreal Engine C++ practices**
- Develop a **system-driven gameplay framework** suitable for complex projects
