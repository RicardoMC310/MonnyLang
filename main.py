import sys

buffer = ""
for line in sys.stdin:
    buffer += line
    try:
        exec(buffer, globals(), globals())
        buffer = ""
    except SyntaxError:
        # Ainda não é um bloco completo, espera a próxima linha
        continue
    except Exception as e:
        print(f"ERROR: {e}")
        buffer = ""
