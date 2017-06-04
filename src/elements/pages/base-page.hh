<?hh

abstract class :base-page extends :x:element {
  protected Vector<string> $path = Vector {};
  protected Map<string, string> $get_data = Map {};
  protected Map<string, string> $post_data = Map {};

  public function set_request_info(Vector<string> $path, Map<string, string> $get_data, Map<string, string> $post_data) {
    $this->path = $path;
    $this->get_data = $get_data;
    $this->post_data = $post_data;
  }
}
