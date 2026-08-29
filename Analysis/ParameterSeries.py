from typing import List


class ParameterSeries:
    @staticmethod
    def arithmetic(min: int, max: int, increment: int) -> List[int]:
        """
        Args:
            min: Minimum value (inclusive)
            max: Maximum value (inclusive)
            increment: Distance between values 
        """
        return [i for i in range(min, max, increment)] + [max]


    @staticmethod
    def geometric(min: int, max: int, range: int) -> List[int]:
        """
        Args:
            min: Minimum value (inclusive)
            max: Maximum value (inclusive)
            range: Number by which to multiply from one number to the next
        """
        res = []
        n = 0
        while n < max:
            res.append(i)
            n *= range
        res.append(n)
        return res

    