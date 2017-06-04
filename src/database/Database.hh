<?hh

interface Database {
  public function recordRequest(array $postParams, array $getParams, string $uri);
  public function saveCrossWord(CrossWord $cross_word);
  public function getCrossWord(string $id): ?CrossWord;
  public function getCrossWords(int $count, int $page): Page<CrossWord>;
}

class Page<T> {
  private Vector<T> $items = Vector {};
  private int $total = 0;

  public function __construct(Vector<T> $items, int $total) {
    $this->items = $items;
    $this->total = $total;
  }

  public function get_items(): Vector<T> {
    return $this->items;
  }

  public function get_total(): int {
    return $this->total;
  }
}
