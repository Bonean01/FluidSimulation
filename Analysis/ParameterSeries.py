from typing import List


class ParameterSeries:
    @staticmethod
    def arithmetic(min: int, max: int, increment: int) -> List[int]:
        """
        Args:
            min: Minimum value (inclusive)
            max: Maximum value (always included)
            increment: Distance between values 
        """
        if increment < 1:
            return
    
        return [i for i in range(min, max, increment)] + [max]


    @staticmethod
    def geometric(min: int, max: int, range: int) -> List[int]:
        """
        Args:
            min: Minimum value (inclusive)
            max: Maximum value (always included)
            range: Number by which to multiply from one number to the next
        """
        if range < 2:
            return
        
        res = []
        n = 1
        while n < max:
            res.append(n)
            n *= range
        res.append(max)
        return res
    