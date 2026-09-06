

class CaseRegistry:
    # Instead of importing every case as a module and hardcoding them here, it would probably
    # be better to force every validation case and benchmark to register themselves into the
    # registry through an abstract method


    @staticmethod
    def detect_validation_case_from_name(name: str):
        """
        Returns the ValidationCase child class type given its name in a kebab-case string
        """
        from Analysis.Validation.LidDrivenCavity import LidDrivenCavity
        from Analysis.Validation.PoiseuilleFlow import PoiseuilleFlow

        case_class = None
        match name:
            case "lid-driven-cavity":
                case_class = LidDrivenCavity
            case "poiseuille-flow":
                case_class = PoiseuilleFlow
            case _:
                raise ValueError(f"The provided validation case: {name} was not recognized")
            
        return case_class


    @staticmethod
    def detect_benchmark_from_name(name: str):
        """
        Returns the Benchmark child class type given its name in a kebab-case string
        """
        from Analysis.Benchmarks.ThreadScaling import ThreadScaling
        from Analysis.Benchmarks.GridScaling import GridScaling

        benchmark_class = None
        match name:
            case "thread-scaling":
                benchmark_class = ThreadScaling
            case "grid-scaling":
                benchmark_class = GridScaling
            case _:
                raise ValueError(f"The provided benchmark case: {name} was not recognized")
            
        return benchmark_class
