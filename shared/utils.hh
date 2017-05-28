<?hh

interface Printable {
  public function __toString(): string;
}

function print_vector<T as Printable>(Vector<T> $vec) {
    foreach ($vec as $value) {
      echo $value . "\n";
    }
}
