<?hh

class NoCommonLetters extends CrossWordGenerationException {}
class AllCommonLettersOccluded extends CrossWordGenerationException {}

class CannotPlaceWord extends CrossWordGenerationException {
  public function __construct(
    private WordJoinData $join_data,
    private CrossWordString $placing,
    private Vector<CrossWordString> $placed,
    private CrossWordGrid $grid) {
    parent::__construct($placing, $placed, $grid);
  }

  public function get_join_pair(): WordJoinData {
    return $this->join_data;
  }
}

class CrossWordGenerationException extends Exception {
  public function __construct(
  private CrossWordString $placing,
  private Vector<CrossWordString> $placed,
  private CrossWordGrid $grid) {
    parent::__construct();
  }

  public function get_placing(): CrossWordString {
    return $this->placing;
  }

  public function get_placed(): Vector<CrossWordString> {
    return $this->placed;
  }

  public function get_grid(): CrossWordGrid {
    return $this->grid;
  }
}

class CrossWordCreationFailed extends Exception {
  public function __construct(private Vector<CrossWordGenerationException> $attempts) {
    parent::__construct();
  }

  public function get_attempts(): Vector<CrossWordGenerationException> {
    return $this->attempts;
  }
}
