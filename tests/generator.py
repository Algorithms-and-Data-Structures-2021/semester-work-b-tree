from random import randint


def values_generator(elements_amount: int, min_value: int = 0, max_value: int = 100000):
    with open(str(elements_amount) + ".csv", "a") as inp:
        for i in range(elements_amount):
            value = randint(min_value, max_value)
            inp.write(str(value) + " ")


if __name__ == '__main__':
    print("Какое количество чисел сгенерировать?")
    elements_amount = int(input())
    values_generator(elements_amount)
