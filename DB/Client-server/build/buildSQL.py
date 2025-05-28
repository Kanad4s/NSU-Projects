import os
import sys

if len(sys.argv) < 2:
    print("Использование: python3 buildSQL.py <pass/to/dirs>")
    sys.exit(1)

root_dir = sys.argv[1]

print(f"Начало обработки папки: {root_dir}")

# Перебираем все элементы в корневой папке
for folder_name in sorted(os.listdir(root_dir), key=str.lower):
    folder_path = os.path.join(root_dir, folder_name)
    
    if os.path.isdir(folder_path):
        print(f"\nОбработка папки: {folder_name}")
        output_file_path = os.path.join(root_dir, f"{folder_name}.sql")

        with open(output_file_path, "w", encoding="utf-8") as output_file:
            # Получаем и сортируем файлы лексикографически
            files = sorted(
                [f for f in os.listdir(folder_path) 
                    if os.path.isfile(os.path.join(folder_path, f))],
                key=str.lower
            )
            
            if not files:
                output_file.write("В этой папке нет файлов\n")
                continue
            
            for file_name in files:
                file_path = os.path.join(folder_path, file_name)
                print(f"  Чтение файла: {file_name}")
                
                try:
                    with open(file_path, "r", encoding="utf-8") as input_file:
                        content = input_file.read()
                        # output_file.write(f"--- {file_name} ---\n")
                        output_file.write(content + "\n\n")
                except UnicodeDecodeError:
                    print(f"--- {file_name} ---\n")
                    print("(бинарный файл, содержимое пропущено)\n\n")
                except Exception as e:
                    print(f"--- {file_name} ---\n")
                    print(f"(ошибка чтения: {str(e)})\n\n")

print("\nОбработка завершена, файлы созданы")