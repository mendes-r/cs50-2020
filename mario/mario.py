from cs50 import get_int


while True:  # Check input. Must be between 0 and 9

    heigth = get_int("Heigth: ")
    if heigth > 0 and heigth < 9:
        break

for i in range(heigth):  # Rows

    for j in range(heigth + 3 + i):

        if j == heigth or j == heigth + 1 or j < heigth - (i + 1):
            print(" ", end="")

        else:
            print("#", end="")

    print()