#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <stdlib.h>
#include <string.h> 

#include <linux/bpf.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

int main() {
    struct bpf_prog_info info;
    __u32 info_len = sizeof(info);
    int stats_fd = bpf_enable_stats(BPF_STATS_RUN_TIME);
    if (stats_fd < 0) {
        printf("Failed to enable the stats with err %d\n", stats_fd);
        return 1;  // Return error code
    }

    struct bpf_object *obj = bpf_object__open("long_running1.kern.o"); 
    if (!obj) {
        printf("Failed to open BPF object\n");
        return 1;
    }

    if (bpf_object__load(obj)) {
        printf("Failed to load the program\n");
        goto cleanup;
    }

    struct bpf_program *prog1 = bpf_object__find_program_by_name(obj, 
                                "tracepoint_exit_saterm");
    if (!prog1) {
        printf("Failed to find the prog1\n");
        goto cleanup;
    }

    int prog_fd = bpf_program__fd(prog1);
    if (prog_fd < 0) {  // Fixed: check for < 0, not == 0
        printf("Failed to get program fd: %d\n", prog_fd);
        goto cleanup;
    }

    struct bpf_link *link1 = bpf_program__attach(prog1);
    if (libbpf_get_error(link1)) {
        printf("Failed to attach the program1\n");
        bpf_link__destroy(link1);
        link1 = NULL;
        goto cleanup;
    }
    
    printf("Attachment of program is done\n");
    printf("Collecting data every second\n");
    printf("timestamp\tavg_runtime_us\ttotal_runtime_sec\trun_cnt_delta\ttotal_run_cnt\n");
    
    __u64 prev_run_time_ns = 0;
    __u64 prev_run_cnt = 0;
    time_t start_time = time(NULL);
    
    while (1) {
        memset(&info, 0, info_len);
        int err = bpf_prog_get_info_by_fd(prog_fd, &info, &info_len);
        if (err < 0) {
            printf("Failed to fetch bpf prog info with err %d\n", err);
            break;
        }
        
        if (info.run_cnt > prev_run_cnt) {  // Changed condition
            time_t current_time = time(NULL);
            __u64 curr_run_cnt = info.run_cnt - prev_run_cnt;
            __u64 curr_run_time_ns = info.run_time_ns - prev_run_time_ns;
            
            // Calculate average runtime per execution in microseconds
            double avg_runtime_us = 0.0;
            if (curr_run_cnt > 0) {
                avg_runtime_us = (double)curr_run_time_ns / curr_run_cnt / 1000.0;
            }
            
            // Total runtime in seconds
            double total_runtime_sec = (double)info.run_time_ns / 1000000000.0;
            
            printf("%ld\t\t%.2f\t\t%.6f\t\t%llu\t\t%llu\n", 
                   current_time - start_time,
                   avg_runtime_us,
                   total_runtime_sec,
                   curr_run_cnt,
                   info.run_cnt);
            
            prev_run_time_ns = info.run_time_ns;
            prev_run_cnt = info.run_cnt;
        }
        
        sleep(1);
    }
    
    // Cleanup link
    if (link1) {
        bpf_link__destroy(link1);
    }
    
cleanup:
    bpf_object__close(obj);
    close(stats_fd);  // Close stats fd
    return 0;
}
