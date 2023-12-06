# Incomplete, the join method ought to make it possible to run this much faster than in answer.py
# but I didn't finish it.

import bisect
from dataclasses import dataclass

@dataclass
class AlmanacEntry:
    src_start: int
    src_end: int
    modifier: int

    def __lt__(self, other):
        return self.src_start < other.src_start

    def munge(self, item: int):
        if self.src_start <= item <= self.src_end:
            return item + self.modifier
        return item

class Almanac:
    def __init__(self, name):
        self.name = name
        self.entries: list[AlmanacEntry] = []

    def __repr__(self):
        return f"Almanac({self.name}, {self.entries})"

    def add(self, item):
        self.entries.append(AlmanacEntry(item[0], item[0] + item[2], item[1] - item[0]))
        self.entries.sort()

    def fetch(self, item: int):
        index = bisect.bisect_left(self.entries, item, key=lambda x: x.src_start)

        if index == len(self.entries) or self.entries[index].src_start > item:
            index -= 1

        if index < 0:
            return item

        return self.entries[index].munge(item)

    def end(self):
        return self.entries[-1].src_end

    def join(a, b):
        result = Almanac(f"{a.name} joined with {b.name}")

        # Cases:
        # A and B do not overlap (A < B)
        #   AAAA BBBB
        # A and B do not overlap (B < A)
        #   BBBB AAAA
        # A and B overlap (A < B)
        # AAAAAAA
        #    BBBBBBB
        # A and B overlap (B < A)
        #     AAAAAAA
        # BBBBBBB
        # A completely within B
        #   AA
        # BBBBB
        # B completely within A
        # AAAAAA
        #   BBB

        working_item = 0
        full_end = max(a.end(), b.end())

        ai = 0
        bi = 0

        # Need to iterate over range-change-points

        while working_item <= full_end:
            if ai < len(a.entries) and bi < len(b.entries):
                start = min(working_item, a.entries[ai].src_start, b.entries[bi].src_start)
            elif ai < len(a.entries):
                start = min(working_item, a.entries[ai].src_start)
            elif bi < len(b.entries):
                start = min(working_item, b.entries[bi].src_start)
            else:
                start = working_item

            end = full_end
            if ai < len(a.entries) and bi < len(b.entries):
                end = max(a.entries[ai].src_start, b.entries[bi].src_start)
                if start == end:
                    end = min(a.entries[ai].src_end, b.entries[bi].src_end)
            elif ai < len(a.entries):
                end = a.entries[ai].src_end
            elif bi < len(b.entries):
                end = b.entries[bi].src_end
            else:
                end = full_end

            aMod = 0
            bMod = 0
            if ai < len(a.entries) and a.entries[ai].src_start <= end:
                aMod = a.entries[ai].modifier
            if bi < len(b.entries) and b.entries[bi].src_start <= end:
                bMod = b.entries[bi].modifier
            modifier = aMod + bMod

            result.entries.append(AlmanacEntry(start, end, modifier))
            working_item = end + 1
            if ai < len(a.entries) and a.entries[ai].src_end < end:
                ai += 1
            if bi < len(b.entries) and b.entries[bi].src_end < end:
                bi += 1

        return result

a = Almanac("a")
a.add((0, 1, 69))

b = Almanac("b")
b.add((56, 60, 37))

c = Almanac.join(a, b)

def test(num: int):
    iter = b.fetch(a.fetch(num))
    join = c.fetch(num)

    if iter != join:
        print(iter, "!=", join)

for i in range(100):
    test(i)

print(c)
