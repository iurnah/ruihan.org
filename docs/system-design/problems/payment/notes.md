# Payment platform

## Uber Payment Platform

[Youtube](https://youtu.be/MJABqwzBkHs)

* focus on externally facing systems
* Disbursement
* Collection
* Payment Service Provider (PSP) (JSON API, one transaction per request/resposne)
* Bank (File-based API, multiple transactions per request/response)
* things responsible for preventing
    * lack of payment
    * duplicate payments
    * Incorrect currency conversion
    * Incorrect payment
    * Dangling authorization
* ![Architecture](fig/uber-payment-arch.png)
* Storage for multiplexing and retries
    * idempotency/deduplication
    * track each request/response before we make the call to PSP
    * strong consistency
    * ![Uber Storage](fig/uber-storage.png)
* Generate unique external ID
    * ID generation clients (allocate batch of IDs to client)
    * serve IDs in memory (backed up)
    * Can only advance
    * ![Unique ID generation](fig/uber-unique-id-generation.png)
* Deterministic Batching
    * Same group of transaction for every try
    * ![Deterministic batching](fig/uber-deterministic-batching.png)

## GreenField MVP

![GreenField MVP Credit Card Architecture](greenfield-credit-card-arch.png)

### Authorization

* client -> Merchant(POS) -> Acquirer -> Network (MasterCard) -> issuer (bank)

## Shopify eCommerce

### Keywords

* Customers
* Products
* Orders
* Payment
* Shop owners
* Isolation of items
* Plugins
* Storefront v.s. backend API

## Write ahead log (WAL)

The payment transaction need to be logged before the API call is made.

