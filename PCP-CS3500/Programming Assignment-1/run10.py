import subprocess
import time
import matplotlib.pyplot as plt

def run_cpp_program(num, thread_num):
    start_time = time.time()
    subprocess.run(["g++", "DAM1-CS23MTECH02001.cpp", "-o", "cpp_program", "-pthread"])  # Compile the C++ program
    subprocess.run(["./cpp_program", str(num), str(thread_num)])  # Run the compiled program with arguments
    end_time = time.time()
    execution_time = end_time - start_time
    return execution_time

def update_input_params(new_thread_num):
    with open("inp-params.txt", "r") as params_file:
        lines = params_file.readlines()

    lines[1] = f"{lines[1].split()[0]} {new_thread_num}\n"  # Update thread_num in inp-params.txt

    with open("inp-params.txt", "w") as params_file:
        params_file.writelines(lines)

def main():
    thread_num_increment = 5
    max_thread_num = 35
    iterations = 10

    avg_times = []  # To store average execution times for each thread_num
    thread_nums = []

    for thread_num in range(5, max_thread_num + 1, thread_num_increment):
        total_execution_time = 0

        print(f"Iteration - thread_num: {thread_num}")
        print("--------------------")
        
        for iteration in range(iterations):
            with open("inp-params.txt", "r") as params_file:
                lines = params_file.readlines()
                num = int(lines[0].split()[0])  # Read num from inp-params.txt
                print(f"Iteration {iteration + 1} - num: {num}, thread_num: {thread_num}")

            execution_time = run_cpp_program(num, thread_num)  # Pass constant value for "num"
            total_execution_time += execution_time

            if (iteration + 1) % 10 == 0:
                print("--------------------")

        avg_execution_time = total_execution_time / iterations
        avg_times.append(avg_execution_time)
        thread_nums.append(thread_num)

        update_input_params(thread_num)  # Update inp-params.txt with new thread_num

        print(f"Iteration - thread_num: {thread_num} completed.")
        print("--------------------")

    plt.plot(thread_nums, avg_times, marker='o')
    plt.title("Average Execution Time vs. Thread_num")
    plt.xlabel("Thread_num")
    plt.ylabel("Average Execution Time (seconds)")
    plt.grid()
    plt.show()

if __name__ == "__main__":
    main()

