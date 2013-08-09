function OnScheduleChange() {
  var sel = document.getElementById('edit-schedule');
  if (sel.options[sel.selectedIndex].value == Drupal.settings.OnGoingSchedule) {
    document.getElementById('edit-ongoing').style.display = 'block';
  }
  else {
    document.getElementById('edit-ongoing').style.display = 'none';
  }
}

function OnEndDateChange() {
  var txtTime = document.getElementById('edit-end-timeEntry-popup-1');
  if (txtTime.value == '') {
    txtTime.value = document.getElementById('edit-start-timeEntry-popup-1').value;
  }
}