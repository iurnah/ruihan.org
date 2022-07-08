# Design Data-Intensive Applications

## Chapter 1 Reliable, Scalable, and Maintainable Applications

| Requirement     | Aspects                                                       |
|-----------------|---------------------------------------------------------------|
| Reliability     | Tolerating hardware & software faults, Human error            |
| Scalability     | Measuring load & performance, latency percentiles, throughput |
| Maintainability | Operability, simplicity, and evolvability                     |

### Reliability

__Expectations:__

* Performs the function that the user expected.
* Tolerate user making mistakes or using it in an unexpected way.
* Performance is remains good for required use cases, under expected load and data volume.
* Prevent unauthorized access and abuse.

__Concepts:__

* Faults: usually defined as one components deviating from its spec.
* Failure: a whole system stop provide the required service to the user.
* Fault tolerance or resilient: prevention v.s. cure faults.

#### Hardware Faults

#### Software Errors

#### Human Errors

### Scalability

### Maintainability

## Chapter 6 Partitioning

### Partition of key-value data

* Key range partitioning
* Key hash partitioning
* Skewed workload and relieving hot spot

### Partitioning and secondary index

* secondary index by document: the secondary index only contains local partition.
* secondary index by term: the secondary index is global.
