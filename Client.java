import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import java.net.Socket;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.net.ConnectException;


public class Client extends Application{

  //init global variables
  TextField result;
  Socket socket;
  BufferedReader input;
  PrintWriter output;


  private static String[] arguments;

  @Override
  public void start(Stage primaryStage) throws IOException{
    //Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
    Stage window = primaryStage;
    window.setTitle("GUI Client");

    //create the buttons
    Button reqbutton = new Button("Request");
    //Button conbutton = new Button("Connect");

    //create a new choicebox with the following choices
    //year, month, day, hour, minute, second
    ChoiceBox<String> selection = new ChoiceBox<>();

    //get items returns the Observable list object which you cna add items to
    selection.getItems().addAll("Year", "Month", "Day", "Hours", "Minutes", "Seconds");

    //set the default value
    selection.setValue("Year");

    //Create the result label as well as the text field that will recieve the response
    Label reslabel = new Label("Result:");
    result = new TextField();


    //Once a choice has been made in the drop down it will
    //send that choice to the server
    reqbutton.setOnAction(e -> {
      try {
        connectServer();
        getSelection(selection);
      } catch (IOException e1) {
        System.err.println("Error : didn't get the requested data");
      }
    });

    //Create the stage and scene
    VBox layout = new VBox(10);

    layout.setPadding(new Insets(20,20,20,20));
    layout.getChildren().addAll(selection, reqbutton, reslabel, result);

    Scene scene = new Scene(layout, 400,300);
    window.setScene(scene);
    window.show();
  }

  //function used to connect to the server
  private void connectServer() throws IOException{
    //connect to the server
    try {
      socket = new Socket(arguments[0], Integer.parseInt(arguments[1]));
      input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      output = new PrintWriter(socket.getOutputStream(), true);

      System.out.println("Connected to the server");
    } catch (ConnectException e) {
      System.err.println("Error : can't connect to the server");
      System.exit(1);
    }
  }

  //Function that, taking the selected value from the choice box
  //will contact the server and get the specific field requested
  private void getSelection(ChoiceBox<String> selection) throws IOException {
    String dataRequested = selection.getValue();
    char choice = 'N';

    //Send the choice to the server
    switch(dataRequested){
      case "Year" :
      choice = '0';
      break;
      case "Month" :
      choice = '1';
      break;
      case "Day" :
      choice = '2';
      break;
      case "Hours" :
      choice = '3';
      break;
      case "Minutes" :
      choice = '4';
      break;
      case "Seconds" :
      choice = '5';
      break;
    }
    output.println(choice);

    //Listen for answer
    result.setText(input.readLine());

    //Disconnect from the server after data is received
    System.out.println("Disconnected from the server");

    //Close the connection
    input.close();
    output.close();
    socket.close();
  }


  public static void main(String[] args) throws IOException {
    arguments = args;
    launch(args);
  }
}
