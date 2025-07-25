# MasterMind Trader - Project Specification

## Original Project Requirements

This document contains the original project specification and requirements from the development contract.

---

## Job Title:
**C++ Developer Needed for High-Frequency Cross-Asset Trading System (Confidential Strategy)**

## Overview:
We are seeking a **skilled and experienced C++ developer** to build a **confidential, cross-asset, high-frequency trading strategy** we refer to as **"Master Mind."** This strategy is designed for markets such as **Forex, Crypto, and F&O (ATM, ITM, OTM—excluding low liquidity strikes)**.

**Your role will be to code the trading engine, integrate exchange APIs, and ensure full reliability across various brokers and platforms. Deliverables: Fully functional C++ codebase with API integrations, documentation, and a user guide for configuration.**

**Important**: The entry/exit logic and proprietary rules of the strategy will NOT be disclosed publicly. Access to such logic will be provided under strict NDAs only after shortlisting.

---

## Strategy Features (General, Non-Proprietary Description):

* **Multi-Asset Compatibility**: Must support asset-specific capital allocation (e.g., $2,000 to EURUSD, $3,000 to BTCUSD, etc.).
* **Renko-Based Execution**: Uses **Renko chart patterns** to trigger entries/exits.
* **Position Sizing & Risk Management**: Fully dynamic risk-based position sizing. Logic must incorporate capital utilization, daily risk % limits, and automated halts after losses.
* **Advanced Order Handling**: Stop orders, take profit, trailing SL logic, and tick-buffer conditions.
* **Capital Efficiency**: Supports per-counter logic to reassess capital after batches of trades.
* **Fallback to Paper Trading**: In case of consecutive losses, switches to paper trades to assess market conditions before resuming live trading.

---

## Your Responsibilities:

* Develop the strategy engine in **C++** with modular and reusable code.
* Build robust **API integrations** for:
  * Crypto exchanges mentioned below
  * Binance
  * Delta Exchange
  * Deribit
  * Coinbase etc.
  * MT4 & MT5 platforms (via bridge or direct plugin).
* Implement a **margin calculator module** for each instrument selected.
* Include all **risk management**, **order routing**, **execution filters**, and **real-time Renko chart pattern detection** (logic shared later).
* Implement **counter-based capital assessment** logic.
* Build a toggle for **live vs paper trading** fallback.
* Ensure **timing flexibility** (e.g., market hours or user-defined trading windows).
* Optimize for performance and minimal latency.
* Provide future support for existing code **debugging and troubleshooting** (without extra cost).
* Maintain **strict confidentiality** throughout development.

## Ideal Candidate:

* 3+ years in **C++** development, preferably in trading system design.
* Strong understanding of **market APIs**, **multi-threading**, **low-latency programming**, and **financial risk logic**.
* Prior experience working with **crypto exchanges** and **MT4/MT5**.
* Creative, reliable, and able to suggest optimizations.
* Familiarity with **Renko charting** is a **plus**, but not mandatory.
* **Available for support** and **willing to sign NDA**.

---

## Deliverables:

1. Well-documented **C++ codebase** with modular design.
2. Compiled binaries or libraries (as needed).
3. Integrated API bridges with the mentioned brokers.
4. Configuration panel or JSON/YAML-based settings input (for symbol-specific capital apportionment, risk %, tick buffer, etc.).
5. Complete ownership and IP rights for the client.
6. Post-deployment bug fixes & improvements at **no additional cost**.

---

## Confidentiality & Legal Notice:

The **core logic of this strategy is strictly proprietary**. Shortlisted candidates will be required to:

* Sign a **Non-Disclosure Agreement (NDA)** before any detailed access is granted.
* Refrain from sharing any part of the code, idea, or implementation.
* Not reuse any part of the project for personal or professional gain.

Violations will result in **legal action**.

## Budget:
We are looking for **cost-effective but high-quality work**. Please mention:
* Your **estimated cost**
* **Timeline**
* Relevant **experience or previous work samples** (especially with trading platforms)

---

## How to Apply:
If you're confident in your ability to develop a **modular, optimized, cross-platform trading system** in C++, kindly apply with the following:

1. Brief description of your experience.
2. Example projects (especially algo trading-related).
3. Confirmation of your ability to sign an NDA.
4. Estimated time and cost.
5. Mention "**Confidential Strategy Build**" in your proposal to confirm you've read the full brief.

---

## Do NOT Apply If:
* You plan to reuse the code elsewhere.
* You are unable to work under strict confidentiality.
* You are unfamiliar with C++ in performance-critical environments.

---

**Project Analysis**: This project is for creating a professional automated trading system capable of executing high-frequency trades across multiple financial markets using proprietary Renko chart patterns. 