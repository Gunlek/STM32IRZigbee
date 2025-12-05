from AT24C import AT24C


class AT24C08(AT24C):
    def __init__(self):
        super().__init__(4, 256)