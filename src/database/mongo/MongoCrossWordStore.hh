<?hh

class CrossWordStore {
  public MongoDB\BSON\ObjectID $_id;
  public array $words;
  public MongoDB\BSON\UTCDateTime $created;

  public function __construct(CrossWord $cross_word) {
    $this->_id = new MongoDB\BSON\ObjectID();
    $words = $cross_word->get_words();
    $this->words = [];
    $this->created = new MongoDB\BSON\UTCDateTime();
    foreach ($words as $word) {
      $this->words[] = new CrossWordStringStore($word);
    }
  }

  public function get_id() {
    return $this->_id->__toString();
  }
}

class CrossWordStringStore {
  public array $cells;
  public string $word;
  public string $hint;

  public function __construct(CrossWordString $string) {
    $this->word = $string->get_word();
    $this->hint = $string->get_hint();
    $this->cells = [];
    foreach ($string->get_cells() as $cell) {
      $this->cells[] = new CrossWordCellStore($cell);
    }
  }
}

class CrossWordCellStore {
  public string $letter;
  public int $x;
  public int $y;

  public function __construct(CrossWordCell $cell) {
    $this->letter = $cell->get_letter();
    $this->x = $cell->get_x();
    $this->y = $cell->get_y();
  }
}
