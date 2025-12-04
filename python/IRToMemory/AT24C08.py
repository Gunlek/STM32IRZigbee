class AT24C08:
    def __init__(self):
        # --- Generate 4 pages of 256 bytes
        self.pages = [[0 for k in range(256)] for i in range(4)]

    def __str__(self):
        str_mmy = ""
        for page in self.pages:
            str_mmy += str(page) + '\n'
        return str_mmy

    def store(self, page, address, value):
        self.pages[page][address] = value