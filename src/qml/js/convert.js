

function convertTimeString(milliseconds)
{
    var seconds = milliseconds / 1000;
    var minutes = Math.floor(seconds / 60);
    var remainingsecs = Math.floor(seconds - (minutes*60));
    var timeString = (minutes < 10 ? "0" + minutes : minutes) + ":" + (remainingsecs < 10 ? "0" + remainingsecs : remainingsecs);
    return timeString;
}
