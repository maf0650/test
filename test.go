package main

import (
    "fmt"
    "runtime"
    "sync"
    "time"
)

const (
    N = 1000000000
    GoroutineNum = 16
)

func sum(start int, end int, wg *sync.WaitGroup, result chan<- int64) {
    var local_sum int64
    for i := start; i <= end; i++ {
        local_sum += int64(i)
    }
    result <- local_sum
    wg.Done()
}

func main() {
    start := time.Now()

    runtime.GOMAXPROCS(GoroutineNum)
    wg := sync.WaitGroup{}
    result := make(chan int64, GoroutineNum)

    for i := 0; i < GoroutineNum; i++ {
        start := i * (N / GoroutineNum) + 1
        end := (i + 1) * (N / GoroutineNum)
        wg.Add(1)
        go sum(start, end, &wg, result)
    }

    go func() {
        wg.Wait()
        close(result)
    }()

    var total_sum int64
    for r := range result {
        total_sum += r
    }

    fmt.Printf("Total sum: %d\n", total_sum)
    fmt.Printf("Time used: %v\n", time.Since(start))
}

