import argparse

from abc import ABC

from Analysis.Validation.ValidationCase import ValidationCase
from Analysis.Validation.LidDrivenCavity import LidDrivenCavity
from Analysis.Validation.PoiseuilleFlow import PoiseuilleFlow


class Benchmark():
    @classmethod
    def set_up_arguments(cls, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            "--case",
            type=str,
            default=None,
            help="Validation case with which to perform the benchmark",
            required=True
        )
    

    @abstractmethod
    def run(self, validation_case_name: str) -> None:
        case = ValidationCase.detect_validation_case_from_name(validation_case_name)
