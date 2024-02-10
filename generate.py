# 更新函数以生成100个大整数运算例子

def generate_multiple_examples(operation, num_examples=20000):
    import random

    # 打开文件准备写入
    filename = f"tests/{operation}.txt"
    with open(filename, "w") as file:
        for _ in range(num_examples):
            # 生成随机大整数
            a = random.randint(1, 10**100)
            b = random.randint(1, 10**100)

            # 根据运算符进行计算
            if operation == "add":
                c = a + b
            elif operation == "sub":
                c = a - b
            elif operation == "mul":
                c = a * b
            elif operation == "div":
                b = random.randint(1, 10**30)
                c = a // b  # 使用整除以避免小数
                
            # 将结果转换为16进制并写入文件
            hex_a = hex(a)
            hex_b = hex(b)
            hex_c = hex(c)
            file.write(f"{hex_a} {hex_b} {hex_c}\n")

    return filename

# 为每种运算生成100个例子并保存到文件
operations = ["add", "sub", "mul", "div"]
filenames_100 = [generate_multiple_examples(op) for op in operations]

