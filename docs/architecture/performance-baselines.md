# Harvey OS Performance Baselines and Regression Strategy

This document details the Harvey OS strategy for establishing performance baselines, defining reference workloads, and implementing automated regression detection. Its goal is to ensure that Harvey OS meets the performance targets defined in its target use cases and that performance does not degrade unintentionally over time.

This complements the "Performance Engineering Framework" section in the `ARCHITECTURAL_MANIFESTO.md` and provides implementation details referenced by `docs/ci-strategy.md`.

## 1. Performance Philosophy

Harvey OS aims for efficient and predictable performance, prioritizing low latency and high throughput where appropriate for its target use cases. Performance is not just about speed but also about resource utilization (CPU, memory) and responsiveness.

## 2. Reference Workloads

Reference workloads are standardized sets of operations designed to simulate key aspects of Harvey OS's target use cases. These workloads are the primary tool for measuring and tracking performance. Each Tier 1 and Tier 2 architecture must have results for these workloads.

### 2.1. Workload Specification Format

Workloads are defined in a structured format (e.g., YAML) to facilitate parsing and execution by automated test harnesses.

```yaml
# Example Workload Definition:
harvey_reference_workloads:
  - workload_id: "iot_gateway_data_forwarding_udp_burst"
    description: "Simulates an IoT gateway under a burst load of UDP sensor data packets, measuring forwarding throughput and latency."
    use_case_refs: ["Secure Embedded IoT Gateway"] # From ARCHITECTURAL_MANIFESTO.md
    tier_applicability: [1] # Applicable to Tier 1 architectures primarily
    components_stressed: ["kernel_network_stack", "udp_daemon_X", "filesystem_logging_minimal"]
    parameters:
      - name: "packet_size_bytes"
        values: [64, 256, 1024]
      - name: "burst_duration_seconds"
        value: 60
      - name: "packets_per_second_target"
        value: 10000
    metrics:
      - name: "avg_throughput_mbps"
        unit: "Mbps"
        aggregator: "average"
      - name: "p99_latency_ms"
        unit: "ms"
        aggregator: "percentile_99"
      - name: "cpu_utilization_percentage_avg"
        unit: "%"
        aggregator: "average"
      - name: "memory_footprint_delta_mb" # Max increase during test
        unit: "MB"
        aggregator: "max"
    acceptance_criteria: # Per-architecture targets, linking to Manifesto Use Case criteria
      # Example for a specific ARM Cortex-A53 target profile
      "arm_cortex_a53_1.2ghz_512mb":
        - metric: "avg_throughput_mbps"
          condition: ">="
          threshold_value: 100 # For 1024 byte packets
        - metric: "p99_latency_ms"
          condition: "<="
          threshold_value: 75
      # Example for an x86_64 target profile
      "x86_64_generic_2ghz_2gb":
        - metric: "avg_throughput_mbps"
          condition: ">="
          threshold_value: 800 # For 1024 byte packets
        - metric: "p99_latency_ms"
          condition: "<="
          threshold_value: 20

  - workload_id: "kernel_compile_from_clean"
    description: "Measures the time to compile the Harvey OS kernel with a standard debug configuration from a clean state."
    use_case_refs: ["OS Research & Education Platform"]
    tier_applicability: [1, 2]
    components_stressed: ["build_system", "compiler_toolchain", "filesystem_io"]
    metrics:
      - name: "compile_time_seconds"
        unit: "seconds"
    acceptance_criteria:
      "x86_64_developer_reference_machine": # As defined by ARB
        - metric: "compile_time_seconds"
          condition: "<="
          threshold_value: 180 # 3 minutes
      "arm64_raspberry_pi_4_4gb":
        - metric: "compile_time_seconds"
          condition: "<="
          threshold_value: 600 # 10 minutes

  # Add more workloads, e.g., filesystem_stress, network_security_rule_lookup etc.
```

### 2.2. Workload Implementation
*   Workloads will be implemented as scripts (e.g., Shell, Python) that utilize existing system tools or custom-developed test utilities.
*   Each script must be capable of outputting results in a machine-readable format (e.g., JSON) that includes the metric name, value, and unit.
*   These scripts will reside in a dedicated `test/performance/` directory in the Harvey OS repository.

## 3. Performance Baselines

*   For each Tier 1 architecture, a set of "golden" performance results (baselines) for all applicable reference workloads will be established.
*   Baselines are determined by running the workloads on specific, well-defined reference hardware configurations.
*   These baselines are version-controlled and updated by the ARB upon significant hardware changes or after agreeing that a new performance level is the accepted standard (e.g., after major optimizations).

## 4. Automated Regression Detection

Performance regression detection is integrated into the CI/CD pipeline as described in `docs/ci-strategy.md`.

### 4.1. Framework Overview
1.  **CI Trigger:** On every pull request to `main` or nightly, for Tier 1 architectures.
2.  **Environment Setup:** CI runner prepares a clean build environment using the official containerized build images on reference hardware (or pre-validated equivalent VMs).
3.  **Build Harvey OS:** Compile the current commit.
4.  **Execute Workloads:** Run the suite of reference workloads.
5.  **Collect Results:** Gather performance metrics from workload outputs.
6.  **Compare with Baseline:**
    *   Fetch the current version-controlled baseline for the architecture and hardware.
    *   Compare each metric against its baseline value and acceptance criteria from the workload definition.
7.  **Report:**
    *   If any metric regresses beyond a defined threshold (e.g., >5% slower, or fails acceptance criteria), the CI job fails.
    *   A detailed report is generated showing metric-by-metric comparison.

### 4.2. Tooling (Conceptual)
*   `run-benchmarks.sh`: Master script to execute all workloads for a given architecture.
*   `compare-results.py`: Python script to compare current results against baseline JSON/YAML files and determine pass/fail.
*   `generate-perf-report.sh`: Script to create an HTML/Markdown report of performance changes.

### 4.3. Makefile Integration (Conceptual)
A `make perf-test` target could orchestrate this process locally for developers:
```makefile
# In a top-level or test Makefile
HARVEY_PERF_DIR := $(CURDIR)/test/performance
LAST_RELEASE_BASELINE_DIR := $(HARVEY_PERF_DIR)/baselines/$(LAST_RELEASE_TAG)

perf-test: build-release # Ensure system is built
	@$(HARVEY_PERF_DIR)/run-benchmarks.sh --output-dir $(CURDIR)/perf_results
	@$(HARVEY_PERF_DIR)/compare-results.py \
          --current $(CURDIR)/perf_results \
          --baseline $(LAST_RELEASE_BASELINE_DIR)/$(TARGET_ARCH)
	@if [ $$? -ne 0 ]; then \
          echo "Performance regression detected!"; \
          $(HARVEY_PERF_DIR)/generate-perf-report.sh --current $(CURDIR)/perf_results --baseline $(LAST_RELEASE_BASELINE_DIR)/$(TARGET_ARCH) > perf_regression_report.html; \
          exit 1; \
      fi
	@echo "Performance tests passed."
```

## 5. Performance Dashboard (Conceptual)

A web-based performance dashboard will provide a visual overview of performance trends over time.

### 5.1. Specifications
*   **Data Source:** Historical performance data collected from CI runs (stored in a time-series database or structured files).
*   **Visualizations:**
    *   Graphs showing performance of key metrics for each workload on Tier 1 architectures over time (per commit/nightly).
    *   Comparison views (e.g., current `main` vs. last release).
    *   Indication of pass/fail status against baselines.
*   **Filtering:** Ability to filter by workload, architecture, date range.
*   **Accessibility:** Viewable by all developers and the community.
*   **Technology Stack (TBD):** Could range from static site generation with embedded charts (e.g., Jekyll + Chart.js) to a more dynamic solution (e.g., Grafana with a suitable backend).

This framework for performance baselines and regression detection is critical for maintaining Harvey OS's performance goals and providing transparency to its users and developers.
