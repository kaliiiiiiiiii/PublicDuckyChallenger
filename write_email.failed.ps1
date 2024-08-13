# source https://community.spiceworks.com/t/sending-an-email-from-outlook-via-powershell/938248
# documentation at https://learn.microsoft.com/en-us/office/vba/api/outlook.mailitem
# modified by Aurin Aegerter
# issue: CO_E_SERVER_EXEC_FAILURE when creating the COM-Object, see https://support.microsoft.com/en-us/topic/considerations-for-server-side-automation-of-office-48bcfe93-8a89-47f1-0bce-017433ad79e2
# COM support dropped, see https://answers.microsoft.com/en-us/outlook_com/forum/all/new-outlook-interop/bf27f4e1-58a9-484c-9d40-bef4162131d9
$status = (get-process | Where-Object {$_.processname -match 'Outlook'})
$outlook = new-object -comobject outlook.application


$email = $outlook.CreateItem(0)
$email.To = "example@example.com"
# $email.CC = "example@example.com"
$email.Subject = "Form"
$email.Body = "Good afternoon

Please fill in the for form at https://is.gd/tuioj

With kind regards'
Steve"

$email.Send()
IF($status){break}
    else{$outlook.Quit()}