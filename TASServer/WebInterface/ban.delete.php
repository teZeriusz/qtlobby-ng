<?php require("inc/head.php") ?>

<?php 
  function doneButton()
  {
    echo "<a class='button1' href='ban.list.php'>OK</a>";
  }
  
  function returnError($err_msg) 
  {
    printError($err_msg);
    echo "<br />";
    echo "<br />";
    doneButton();
    exit();
  }
  
  function displayDeletionForm()
  {
    echo '<FORM method="post" action="ban.delete.php?id='.$_GET["id"].'&confirm=true">';
    echo '  <table class="table4" width="300">';
    echo '    <tr>';
    echo '      <td style="color:blue">Ban entry ID: '.$_GET["id"].'</td>';
    echo '    </tr>';
    echo '    <tr>';
    echo '      <td>Reason for deletion (optional):';
    echo '        <textarea name="Reason" cols="50" rows="5"></textarea>';
    echo '      </td>';
    echo '    </tr>';
    echo '    <tr>';
    echo '      <td style="text-align: left;">';
    echo '        <p>Confirm deletion:</p>';
    echo '        <input type="submit" value="Delete ban entry" style="width: 150px">';
    echo '      </td>';
    echo '    </tr>';
    echo '  </table>';
    echo '</FORM>';
  }

  if (!$_GET["id"]) {
    returnError("Ban ID is missing!");
  } else if (!($_GET["confirm"] == "true")) {
    displayDeletionForm();
  } else {
    // construct the sql UPDATE statement:
    $update = 'UPDATE BanEntries SET Enabled=0 WHERE id=' . $_GET["id"];
  
    // construct the sql INSERT statement:
    $insert = "INSERT INTO BanDeletionHistory (BanID, Author";
    if ($_POST["Reason"]) $insert .= ', Reason';
    $insert .= ") values (";
    $insert .= '"' . $_GET["id"]. '", "'. $_SESSION['username'] . '"';
    if ($_POST["Reason"]) $insert .= ', "' . $_POST["Reason"] . '"';
    $insert .= ')';

    // now connect to the database and add new ban entry:
    // (Done after this tutorial: http://www.databasejournal.com/features/mysql/article.php/1469211)
    $dbh = mysql_connect($constants['database_url'], $constants['database_username'], $constants['database_password']) 
      or returnError("Unable to connect to the database.");
    $selected = mysql_select_db($constants['database_name'], $dbh) 
      or returnError("Problems connecting to the database.");

    // update Enabled field in BanEntries table:
    if (!mysql_query($update)) {
      returnError("Error while trying to access the database. (UPDATE statement error)");
    }    
    // insert ban entry into the database: 
    if (!mysql_query($insert)) {
      returnError("Error while trying to access the database. (INSERT statement error)");
    }
    
    mysql_close($dbh);
    
    echo "<p>Ban entry successfully deleted.</p>";
    doneButton();
  }

?>

<?php require("inc/footer.php") ?>