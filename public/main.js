React.render = ReactDOM.render;

class CrossWordGame extends React.Component {
  contructor(props) {
  }

  render() {
    return React.createElement("section", {}, "CrossWordGame");
  }
}

window.CrossWordGame = CrossWordGame;
