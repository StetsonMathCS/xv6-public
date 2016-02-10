library(RUnit)

check_sig <- function(d, metric, sched1, sched2, required_pvalue) {
  cat(paste("\n\nRunning t.test between", sched1, "and", sched2, "on metric", metric, "\n"))
  tt <- t.test(d[d$sched == sched1, metric], d[d$sched == sched2, metric])
  print(tt)
  return(tt["p.value"] <= required_pvalue)
}

check_mean_diff <- function(d, metric, sched1, sched2, required_diff) {
  m1 <- mean(d[d$sched == sched1, metric])
  m2 <- mean(d[d$sched == sched2, metric])
  print(paste("Mean of", sched1, "on metric", metric, "is", m1))
  print(paste("Mean of", sched2, "on metric", metric, "is", m2))
  md <- m1 - m2
  print(paste("Mean difference is", md))
  return(md >= required_diff)
}

test.benchmarks <- function() {
  d <- read.csv("sched-benchmarks.csv")

  # total ticks
  checkTrue(check_sig(d, "ticks", "rr", "lifo", 0.01), "Checking that rr and lifo have statistically significantly different total tick counts")
  checkTrue(check_mean_diff(d, "ticks", "lifo", "rr", 50), "Checking that lifo has on average >50 more total ticks than rr")
  checkTrue(check_sig(d, "ticks", "rr", "fifo", 0.01), "Checking that rr and fifo have statistically significantly different total tick counts")
  checkTrue(check_mean_diff(d, "ticks", "fifo", "rr", 50), "Checking that fifo has on average >50 more total ticks than rr")

  # procticks
  for(sched in c("rr", "fifo", "mlfq")) {
    checkTrue(check_sig(d, "procticks", "lifo", sched, 0.01), paste("Checking that lifo and", sched, "have statistically significantly different total proc tick counts"))
    checkTrue(check_mean_diff(d, "procticks", sched, "lifo", 700), paste("Checking that", sched, "has on average >700 more total proc ticks than lifo"))
  }
  for(sched1 in c("rr", "fifo", "mlfq")) {
    for(sched2 in c("rr", "fifo", "mlfq")) {
      if(sched1 != sched2) {
        checkTrue(!check_sig(d, "procticks", sched1, sched2, 0.05), paste("Checking that", sched1, "and", sched2, "do not have statistically significantly different total proc tick counts"))
      }
    }
  }
  checkTrue(mean(d[d$sched == "lifo", "procfirstrun"]) < 5, "Checking that lifo has on average <5 procfirstrun value")
  checkTrue(mean(d[d$sched == "mlfq", "procfirstrun"]) < 5, "Checking that mlfq has on average <5 procfirstrun value")
  checkTrue(check_sig(d, "procfirstrun", "lifo", "rr", 0.01), "Checking that lifo and rr have statistically significantly different procfirstrun values")
  checkTrue(check_sig(d, "procfirstrun", "mlfq", "rr", 0.01), "Checking that mlfq and rr have statistically significantly different procfirstrun values")
  checkTrue(check_sig(d, "procfirstrun", "lifo", "fifo", 0.01), "Checking that lifo and fifo have statistically significantly different procfirstrun values")
  checkTrue(check_sig(d, "procfirstrun", "mlfq", "fifo", 0.01), "Checking that mlfq and fifo have statistically significantly different procfirstrun values")
  checkTrue(check_sig(d, "procfirstrun", "rr", "fifo", 0.01), "Checking that rr and fifo have statistically significantly different procfirstrun values")
}

testsuite.benchmarks <- defineTestSuite("benchmarks", dirs=".")


