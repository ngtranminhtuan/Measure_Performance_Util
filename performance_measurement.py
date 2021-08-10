from tabulate import tabulate
import time

class Logging:
    def __init__(self):
        self.data = {}
        self.sum_data = {}
        self.counter_data = {}
    
    def start(self, name):
        t = time.time()
        self.data[name] = [t]

        if name not in self.sum_data:
            self.sum_data[name] = [0]
            self.counter_data[name] = [0]

    def end(self, name):
        elapsed_time = round(time.time() - self.data[name][0], 3)
        self.data[name][0] = elapsed_time

        # Add into sum_data
        self.sum_data[name][0] += elapsed_time
        self.counter_data[name][0] += 1

        # Ignore small value
        if self.data[name][0] < 0.001:
            self.data[name][0] = 0        

    def add(self, name, time):
        self.data[name][0] += round(time, 3)

    def sub(self, name, time):
        self.data[name][0] -= round(time, 3)

    def mul(self, name, ratio):
        self.sum_data[name][0] += (self.data[name][0]*ratio) - (self.data[name][0])
        self.data[name][0] *= ratio
        self.data[name][0] = round(self.data[name][0], 3)

    def div(self, name, ratio):
        self.data[name][0] /= ratio
        self.data[name][0] = round(self.data[name][0], 3)

    def print_result(self):
        print(tabulate(self.data,headers=self.data.keys(), tablefmt="fancy_grid"))

    def print_mean_result(self):
        mean_data = {}
        for key in self.sum_data.keys():
            mean_data[key] = [0]
            mean_data[key][0] = round(self.sum_data[key][0] / self.counter_data[key][0], 3)
        print(tabulate(mean_data,headers=mean_data.keys(), tablefmt="fancy_grid"))

if __name__ == "__main__":
    log = Logging()

    for i in range(2):
        log.start("Inference time")
        print("Test function 1 running...")
        time.sleep(i)
        log.end("Inference time")

        log.start("Postprocess time")
        print("Test function 2 running...")
        time.sleep(i+1)
        log.end("Postprocess time")

        log.print_result()

        print("Average Time")
        log.print_mean_result()

    print("Average Time")
    log.print_mean_result()

    
    



