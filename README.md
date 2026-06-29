# Strict Context Isolation Architecture (SCIA)

### 1. Bounded Context as an Isolated Unit
* **Context:** Domain-driven architectural organization to eliminate code coupling and state leaks.
* **Decision:** Every business context is a dedicated class inheriting from a base context class.
* **Why:** Establishes rigid architectural boundaries. Any attempt to bypass the interface to interact with a context indicates a design flaw.

### 2. Exclusive Entry Points: CommandService and QueryService
* **Context:** Context interaction, state mutation, and data retrieval.
* **Decision:** `CommandService` and `QueryService` are the **only** entry points. A context may implement one or both. Every command and query must reside within these services; no other interaction path exists.
* **Why:** Guarantees absolute encapsulation. Framework-level enforcement prevents unauthorized state mutations and restricts direct access to domain internals.

### 3. DI Container and UI Composition
* **Context:** Context lifecycle management and dependency injection.
* **Decision:** Contexts are registered within a central `DIContainer`. The `UIComposer` instantiates ViewModels, injecting the `Context` instances.
* **Why:** The `Context` acts as a public gateway that exposes only its `CommandService` and `QueryService` interfaces. Internal domain mechanics (like repositories) remain encapsulated and private, ensuring the UI layer cannot access or mutate domain state directly.