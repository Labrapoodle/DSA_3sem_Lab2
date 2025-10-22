import random
import time


def main():
    print("\n============================\n"
          "This program generate <n> random words with generator's seed <seed>\n"
          "from <min lenght> to <max length>, from <min ascii> to <max ascii>\n"
          "and write it into <filename>;\n"
          "if generator's seed equals to 0, then seed is current time\n"
          "============================\n")
    
    n, min_length, max_length, min_ascii, max_ascii, seed = \
        map(int, input("Input:\n<n> <min length> <max length> <min ascii> <max ascii> <seed>\n").split())
    
    if (max_length < min_length or max_ascii < min_ascii):
        print("<max lenght> should be less than or equal to <min lenght>\n"
              "<max ascii> should be less than or equal to <min ascii>\n")
        exit(1)
    
    if (max_length < 1 or min_length < 1 or n < 1 or min_ascii < 32 or max_ascii < 32
            or min_ascii > 126 or max_ascii > 126):
        print("all numbers should be greather than 0;\n"
              "<max ascii> and <min ascii> should be less than 126 and greather than 31;\n")
        exit(1)
    
    if (seed == 0):
        random.seed(time.time())
    else:
        random.seed(seed)
    
    if (n > 5000000):
        print("n > 5000000, exit\n")
        exit(1)
    elif (n >= 1000000):
        res = input("n = " + str(n) + ", press <Y> to continue\n")
        if (res != "Y"):
            exit(1)
    
    filename = input("Input <filename>:\n")
    file = open(filename, 'w')
    
    for i in range(0, n):
        word_len = random.randint(min_length, max_length)
        word = ""
        for j in range(0, word_len):
            word += chr(random.randint(min_ascii, max_ascii))
        word += '\n'
        file.write(word)
    
    file.close()
    print("Done\n")


main()