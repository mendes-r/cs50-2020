from cs50 import get_string


def main():

    number = get_string("Insert Card Number: ")
    checkCreditNumber(number)  # Start the checking function


def checkCreditNumber(number):

    endSum = 0  # Sum for the Luhn’s Algorithm
    counter = 0  # Diferenciate between which order to use
    counterTwoFirst = 0
    firstTwoDigits = int(number[0]) * 10 + int(number[1])
    numberLength = len(number)

    if numberLength % 2 != 0:  # If odd let s start the counter at 1 because we should double de de digit starting from the secound-last
        counter = 1

    for n in map(int, number):  # Iterate througth the number starting at the last digit

        if counter % 2 == 0:

            if n * 2 < 10:
                endSum += (n*2)
            else:
                endSum += 1 + (n*2) % 10

        else:
            endSum += n

        counter += 1

    if numberLength >= 13 and numberLength <= 16 and (endSum % 10) == 0:  # Validate and give a name to the card

        if firstTwoDigits in [34, 37]:
            print("AMEX")

        elif firstTwoDigits in [51, 52, 53, 54, 55]:
            print("MASTERCARD")

        elif int(firstTwoDigits / 10) == 4:
            print("VISA")

        else:
            print("INVALID")

    else:
        print("INVALID")


main()  # Start program