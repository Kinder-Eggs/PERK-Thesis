# PERK-Thesis
Implementations of the PERK algorithm optimized for ARMv8.5-A architecture (Apple M1 processor) for my bachelor thesis

For compiling, use `make VERSION=<FAST,SHORT><1,3,5><3,5>` to select version, example: `make VERSION=FAST13`
Then use `./main` for testing with KATs or `sudo ./bench` to run the benchmark

The written thesis can be found [here](https://www.ic.unicamp.br/~reltech/PFG/2023/PFG-23-42.pdf) (Portuguese only)
