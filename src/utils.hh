<?hh

interface Printable {
  public function __toString(): string;
}

function print_vector<T as Printable>(Vector<T> $vec) {
    foreach ($vec as $value) {
      echo $value . "\n";
    }
}

function shuffle_vec<T>(Vector<T> $vec) {
    for ($i=0; $i<$vec->count(); $i++) {
      $roll = rand($i, $vec->count()-1);
      $temp = $vec[$i];
      $vec[$i] = $vec[$roll];
      $vec[$roll] = $temp;
    }
}
