# How to Design a Ticket System

Similar design questions

* How to design a coupon system (ticketmaster)

## System Requirements

* What's type of coupon?
    * Free or non free? (payment system)
    * Can it be shared?
    * Does it have an expiration?
    * What's other features it should support beside distributing coupons?
* Users
    * clients, consume coupon/tickets
    * vendors, produce coupon/tickets, need to have a coupon __management__
      system to do that.
* coupon "transactions"
* coupon verification, barcode, etc.
* Availablility
* Fault torlerance
* Scalability

## Core features

* Browse coupon by shop, product/service types (categories).
* Claim a coupon, put into their bag.
* how to get a coupon?
* Who and how to produce a coupon?
* user profile/account?
* Handle failure
* Handle coupon timeout
* Horizontal scale

## Data structure and calculations

* How many products, how many coupons for each product?
* Time range we are publishing coupones? (1 month, 1 week?)
* How many coupon in total we need to tracking at the same time?
* distributed queue data structure
* elements in the queue should be a coupon object (with type, counter, expiration, etc.)
* May need to have different queue to keep coupon in different stages (e.g. created,
  published (display), reserved, claimed, expired).

## MVP

* overall system architecture,
* database Schema design (the data model and __data size__)
    * coupon (128 bytes)
    * product (128 bytes)
    * ticket event table
    * event seats table
    * user table
    * purchase info table
* How to handle two people try to purchase the same ticket?
* How to store ticket purchase information in the database?
    * can store with seat table.
    * can store the credit card within the user profile.
    * can have a seperate table for purchase, flexible in handling return and refund.
* How to handle guest purchase?
    * Is register required?
    * anonymous with purchase info, events, tickets, etc.
* Walk thorugh a simple use case to check whether we missed any core feature.
  
## Scale it

* reflect on the QPS calculation, identify bottlenecks and issues for high QPS.
  * load balancer
  * cache (CDN)
* how to handle different people by the same ticket?
    * draw a timeline to show the potentiall issue (race condition)
    * how to ensure there is no race condition?
        * check in each step whether the seat is available or not? (browse, add payment,
          submit order)
        * ACID database with transaction support (who ever comes first get the ticket)
* how to improve the user experience by deciding when to `lock` the seat.
  whoever first select the seat will be sure they will get it, but bad guys might
  never buy but just select. How can we solve the issue?
    * timeout, after timeout, if not paid, go back to the queue so that others
      can buy.
* Database performance
    * Read v.s write
    * If read heavy, memory cache can be handy (Memcached)
* Frontend performance
    * autoscaler (auto add additional webserver)
    * load balancer (horizontal scale)
    * cache static html file (precompute the main listing page cache it in memory or CDN)
* Reliability
    * identify Single point of failure
    * add backups/replica/failovers (loadbalancer, database, distributed cache)
    * RAM volatile memory, lost data on power outage. (Uninterruptible power supply (UPS))

## Bonus

* support return and refund?
* handle peak traffic incase a popolar event coming up.
* How to design firewall/routing policy, and how to secure user password and information
* Enable SSL for secure client server data transfer.

## Reference

* [How We Developed Scalable Coupon Management System In Node?](https://medium.com/@STYLABSHQ/how-we-developed-scalable-coupon-management-system-in-node-945426b02df1)
* 