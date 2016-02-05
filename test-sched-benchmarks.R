library(RUnit)

testsuite.benchmarks <- defineTestSuite("benchmarks", dirs=".")

runTestSuite(testsuite.benchmarks)

