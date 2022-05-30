import sys


class bar:
    def __init__(self, length):
        self.length = length

    def print(self):
        print("[" + "-" * self.length + "]")
        return

    def update(self, progress):
        sys.stdout.write("\033[F")
        if progress > 1:
            progress = 1
        progress_line = "#" * round(self.length * progress) + "-" * int(self.length * (1 - progress))
        print("[" + progress_line)
