#include <memory>

#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.Foundation.h>

#include <iostream>
#include <locale>
#include <codecvt>

#include "shortcutcreator.hpp"
#include "win32_api.hpp"

int main()
{
  try
  {
    using namespace winrt::Windows::UI::Notifications;

    winrt::init_apartment();

    const std::wstring aumid = L"lag.Testing.Notifications"; //application user model ID
    auto m = TryCreateShortcut(aumid);
    if (m == 0 /* S_OK */ && SetAppModelID(aumid))
    {
      auto history = ToastNotificationManager::History();
      auto notifier = ToastNotificationManager::CreateToastNotifier(aumid);

      auto supports_modern = SupportsModernNotifications();

      winrt::Windows::Data::Xml::Dom::XmlDocument doc;
      {
        doc.LoadXml(L"<toast>\
                        <visual>\
                            <binding template=\"ToastGeneric\">\
                                <text></text>\
                                <text></text>\
                                <image placement=\"appLogoOverride\" hint-crop=\"circle\"/>\
                                <image/>\
                            </binding>\
                        </visual>\
                        <actions>\
                            <input\
                                id=\"tbReply\"\
                                type=\"text\"\
                                placeHolderContent=\"Type a reply\"/>\
                            <action\
                                content=\"Reply\"\
                                activationType=\"background\"/>\
                            <action\
                                content=\"Like\"\
                                activationType=\"background\"/>\
                            <action\
                                content=\"View\"\
                                activationType=\"background\"/>\
                        </actions>\
                    </toast>"
          );

          doc.DocumentElement().SetAttribute(L"launch", L"action=viewConversation&conversationId=9813");
          doc.SelectSingleNode(L"//text[1]").InnerText(L"Andrew sent you a picture");
          doc.SelectSingleNode(L"//text[2]").InnerText(L"Check this out, Happy Canyon in Utah!");
          doc.SelectSingleNode(L"//image[1]")
            .as<winrt::Windows::Data::Xml::Dom::XmlElement>()
            .SetAttribute(L"src", L"https://unsplash.it/64?image=1005");
          doc.SelectSingleNode(L"//image[2]")
            .as<winrt::Windows::Data::Xml::Dom::XmlElement>()
            .SetAttribute(L"src", L"https://picsum.photos/364/202?image=883");
          doc.SelectSingleNode(L"//action[1]")
            .as<winrt::Windows::Data::Xml::Dom::XmlElement>()
            .SetAttribute(L"arguments", L"action=reply&conversationId=9813");
          doc.SelectSingleNode(L"//action[2]")
            .as<winrt::Windows::Data::Xml::Dom::XmlElement>()
            .SetAttribute(L"arguments", L"action=like&conversationId=9813");
          doc.SelectSingleNode(L"//action[3]")
            .as<winrt::Windows::Data::Xml::Dom::XmlElement>()
            .SetAttribute(L"arguments", L"action=viewImage&imageUrl=https://picsum.photos/364/202?image=883");
      }

      auto toastdoc = ToastNotificationManager::GetTemplateContent( ToastTemplateType::ToastText01 );
      toastdoc.SelectSingleNode(L"//text").InnerText(L"hello world");

      struct notification_data
      {
        ToastNotification toast;
        winrt::event_token activated_token;
        winrt::event_token dismissed_token;
        winrt::event_token failed_token;
      };
      auto notification = notification_data { ToastNotification { doc } };
      {
        std::shared_ptr<int> ptr_test(
          new int,
          [](int* p)
            {
              delete p;
              std::cout << "Deleted :D" << std::endl;
            }
        );

        notification.activated_token = notification.toast.Activated([ptr_test](auto sender, auto inspectable)
        {
          std::cout << "Notification activated!" << std::endl;
          int m = *ptr_test;
        });

        notification.dismissed_token = notification.toast.Dismissed([ptr_test](auto sender, auto inspectable)
        {
          std::cout << "Notification dismissed :(" << std::endl;
          int m = *ptr_test;
        });

        notification.failed_token = notification.toast.Failed([ptr_test](auto sender, auto inspectable)
        {
          std::cout << "Notification failed :(" << std::endl;
          int m = *ptr_test;
        });

        notifier.Show(notification.toast);
      }

      wchar_t d;
      std::wcin >> d;

      // cleanup
      notification.toast.Activated(notification.activated_token);
      notification.toast.Dismissed(notification.dismissed_token);
      notification.toast.Failed(notification.failed_token);

      std::wcin >> d;
    }
  }
  catch ( const winrt::hresult_error &e )
  {
    std::wcerr <<"error 0x"<< std::hex << e.code() <<": "<<
      std::wstring_view{e.message()} << '\n';
    return 1;
  }
  catch ( const std::exception &e )
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}