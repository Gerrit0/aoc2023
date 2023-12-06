import bisect
import itertools

input_file = "sample.txt"
input_file = "input.txt"

seeds = list()

class Almanac:
    def __init__(self, name):
        self.name = name
        self.entries = []

    def __repr__(self):
        return f"Almanac({self.name}, {self.entries})"

    def add(self, item):
        self.entries.append(tuple(item))
        self.entries.sort()

    def fetch(self, item: int):
        entry = bisect.bisect_left(self.entries, item, key=lambda x: x[0])

        if entry == len(self.entries) or self.entries[entry][0] > item:
            entry -= 1

        if entry < 0:
            return item

        in_low, out_low, size = self.entries[entry]
        if in_low <= item < in_low + size:
            return out_low + (item - in_low)

        # for in_low, out_low, size in self.entries:
        #     if in_low <= item < in_low + size:
        #         return out_low + (item - in_low)
        return item

seed_to_soil = Almanac("Seed to soil")
soil_to_fert = Almanac("Soil to fert")
fert_to_water = Almanac("Fert to water")
water_to_light = Almanac("Water to light")
light_to_temp = Almanac("Light to temp")
temp_to_humid = Almanac("Temp to humid")
humid_to_loc = Almanac("Humid to loc")

active_almanac = None
with open(input_file, "r") as f:
    for line in f.readlines():
        if not line.strip():
            continue
        if "seeds:" in line:
            seeds = [int(x) for x in line[line.index(":") + 1:].split()]
        elif "seed-to-soil map:" in line:
            active_almanac = seed_to_soil
        elif "soil-to-fertilizer map:" in line:
            active_almanac = soil_to_fert
        elif "fertilizer-to-water map:" in line:
            active_almanac = fert_to_water
        elif "water-to-light map:" in line:
            active_almanac = water_to_light
        elif "light-to-temperature map:" in line:
            active_almanac = light_to_temp
        elif "temperature-to-humidity map:" in line:
            active_almanac = temp_to_humid
        elif "humidity-to-location map:" in line:
            active_almanac = humid_to_loc
        else:
            out_low, in_low, size = [int(x) for x in line.split()]
            active_almanac.add((in_low, out_low, size))


def resolve(seed: int):
    soil = seed_to_soil.fetch(seed)
    fert = soil_to_fert.fetch(soil)
    water = fert_to_water.fetch(fert)
    light = water_to_light.fetch(water)
    temp = light_to_temp.fetch(light)
    humid = temp_to_humid.fetch(temp)
    loc = humid_to_loc.fetch(humid)

    # print("====", seed, soil, fert, water, light, temp, humid, "=>", loc)
    return loc

def run_range(args):
    start, size = args
    min_so_far = 99999999999999999
    for i in range(size):
        min_so_far = min(min_so_far, resolve(start + i))
    return min_so_far


# Part 1
# for seed in seeds:
#     min_so_far = min(min_so_far, resolve(seed))

if __name__ == "__main__":
    # Part 2
    print(len(seeds) / 2)
    from multiprocessing import Pool
    with Pool(10) as pool:
        mins = pool.map(run_range, itertools.batched(seeds, 2))
        print(mins)

# for seed, size in itertools.batched(seeds, 2):
#     print(seed, size)
#     for i in range(size):
#         if i % 1_000_000 == 0:
#             print(i)
#         min_so_far = min(min_so_far, resolve(seed + i))
