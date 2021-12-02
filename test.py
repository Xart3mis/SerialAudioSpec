block = [i * 5.75 for i in range(65)]

els = len(block)
ops = els // 10 if els % 10 == 0 else els // 10 + 1


b = [
    "#" + y if len(y := str(round(i, 3))) < 6 else "#" + str(-1)
    for i in (block[:10] if len(block) >= 10 else block)
]


print(b)

"""

"""
