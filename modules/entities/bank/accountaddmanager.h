#ifndef ACCOUNTADDMANAGER_H
#define ACCOUNTADDMANAGER_H

#include "IHistoryManager.h"
#include "accountadd.h"

class AccountAddManager : public IHistoryManager {
  Q_OBJECT
public:
  static bool deposit_request(size_t initiator_id, size_t bank_id,
                              size_t account_id, size_t period_in_month,
                              size_t percent);
  static bool salary_request(size_t initiator_id, size_t bank_id,
                             size_t account_id, size_t period_in_month,
                             size_t salary_amount);

  // Manager

  AccountAddManager(IUser *user, AccountAdd::Type add_type);

  std::vector<QTableWidgetItem *> get_items() const override;
  bool mark(size_t item_index, bool verify = false) override;
  size_t get_selected(size_t index) const override;

private slots:
  void update_vars() override;

private:
  AccountAdd::Type add_type;

  // values
  std::vector<std::unique_ptr<AccountAdd>> adds;
  std::vector<std::unique_ptr<Request>> requests;
};

#endif // ACCOUNTADDMANAGER_H
