#include "Frames/FPositionDetails.h"

namespace XQuant {


	enum MyItemColumnID
	{
		MyItemColumnID_ID,
		MyItemColumnID_Name,
		MyItemColumnID_Action,
		MyItemColumnID_Quantity,
		MyItemColumnID_Description
	};

	struct MyItem
	{
		int         ID;
		const char* Name;
		int         Quantity;

		// We have a problem which is affecting _only this demo_ and should not affect your code:
		// As we don't rely on std:: or other third-party library to compile dear imgui, we only have reliable access to qsort(),
		// however qsort doesn't allow passing user data to comparing function.
		// As a workaround, we are storing the sort specs in a static/global for the comparing function to access.
		// In your own use case you would probably pass the sort specs to your sorting/comparing functions directly and not use a global.
		// We could technically call ImGui::TableGetSortSpecs() in CompareWithSortSpecs(), but considering that this function is called
		// very often by the sorting algorithm it would be a little wasteful.
		static const ImGuiTableSortSpecs* s_current_sort_specs;

		// Compare function to be used by qsort()
		static int CompareWithSortSpecs(const void* lhs, const void* rhs)
		{
			const MyItem* a = (const MyItem*)lhs;
			const MyItem* b = (const MyItem*)rhs;
			for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
			{
				// Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
				// We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
				const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
				int delta = 0;
				switch (sort_spec->ColumnUserID)
				{
				case MyItemColumnID_ID:             delta = (a->ID - b->ID);                break;
				case MyItemColumnID_Name:           delta = (strcmp(a->Name, b->Name));     break;
				case MyItemColumnID_Quantity:       delta = (a->Quantity - b->Quantity);    break;
				case MyItemColumnID_Description:    delta = (strcmp(a->Name, b->Name));     break;
				default: IM_ASSERT(0); break;
				}
				if (delta > 0)
					return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
				if (delta < 0)
					return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
			}

			// qsort() is instable so always return a way to differenciate items.
			// Your own compare function may want to avoid fallback on implicit sort specs e.g. a Name compare if it wasn't already part of the sort specs.
			return (a->ID - b->ID);
		}
	};
	const ImGuiTableSortSpecs* MyItem::s_current_sort_specs = NULL;




	FPositionDetails::FPositionDetails(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FPositionDetails::onAttach() {
		_isShow = true;
	}

	void FPositionDetails::onDetach() {

	}

	void FPositionDetails::onEvent(Event& e) {

	}

	void FPositionDetails::onImGuiRender() {
		if (!_isShow) {
			// TOFIX 这地方可以改为事件
			// Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			// ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winWidth) / 2, float(Config::ScreenHeigth - _winHeigth) / 2 });
			_initWinPos = false;
		}

		// 持仓明细界面、包含当前账户资金信息、总体收益率、今日收益率、持仓盈亏、仓位，交易买卖窗口
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);

		// 提示
		// 资产
		ImGui::BeginGroup();
		ImGui::BulletText(u8"总资产: "); ImGui::SameLine(); ImGui::Text(u8"20");
		ImGui::BulletText(u8"总市值: "); ImGui::SameLine(); ImGui::Text(u8"10");
		ImGui::BulletText(u8"总盈亏: "); ImGui::SameLine(); ImGui::Text(u8"3");
		ImGui::EndGroup();

		// 收益率
		ImGui::SameLine();
		ImGui::SetCursorPosX(200.0f);
		ImGui::BeginGroup();
		ImGui::BulletText(u8"总收益率: "); ImGui::SameLine(); ImGui::Text(u8"20");
		ImGui::BulletText(u8"今收益率: "); ImGui::SameLine(); ImGui::Text(u8"10");
		ImGui::BulletText(u8"仓位: "); ImGui::SameLine(); ImGui::Text(u8"3");
		ImGui::EndGroup();

		ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x + 330.0f, ImGui::GetWindowPos().y + 10.0f },
			{ ImGui::GetWindowPos().x + 330.0f, ImGui::GetWindowPos().y + 110.0f }, ImColor(111, 111, 111, 255));

		// 代码
		ImGui::SameLine();
		ImGui::SetCursorPosX(360.0f);
		ImGui::BeginGroup();
		ImGui::SetNextItemWidth(120);
		static char buf1[64] = ""; ImGui::InputText(u8"代码", buf1, 64);
		ImGui::SetNextItemWidth(120);
		static float foo = 12.0f;
		ImGui::InputFloat(u8"价格", &foo, 0.01f, 0);
		ImGui::SetNextItemWidth(120);
		static int foo1 = 100;
		ImGui::InputInt(u8"股数", &foo1, 100, 0);
		ImGui::EndGroup();

		// 操作
		ImGui::SameLine();
		ImGui::SetCursorPosX(550.0f);
		ImGui::BeginGroup();
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 100));
		if (ImGui::Button(u8"买入", ImVec2(80, 23)))
		{

		}
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 255, 0, 100));
		if (ImGui::Button(u8"卖出", ImVec2(80, 23)))
		{

		}
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 255, 100));
		if (ImGui::Button(u8"平仓", ImVec2(80, 23)))
		{


		}
		ImGui::PopStyleColor();
		ImGui::EndGroup();

		// 分割线
		ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 120.0f },
			{ ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + 120.0f }, ImColor(111, 111, 111, 255));

		ImGui::SetCursorPosY(130.0f);
		// 持仓
		static const char* template_items_names[] =
		{
			"Banana", "Apple", "Cherry", "Watermelon", "Grapefruit", "Strawberry", "Mango",
			"Kiwi", "Orange", "Pineapple", "Blueberry", "Plum", "Coconut", "Pear", "Apricot"
		};

		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		static int items_count = IM_ARRAYSIZE(template_items_names) * 2;
		static float inner_width_with_scroll = 0.0f; // Auto-extend
		static bool outer_size_enabled = true;
		static ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);
		static float row_min_height = 0.0f; // Auto
		static int freeze_cols = 1;
		static int freeze_rows = 1;
		enum ContentsType { CT_Text, CT_Button, CT_SmallButton, CT_FillButton, CT_Selectable, CT_SelectableSpanRow };
		static int contents_type = CT_SelectableSpanRow;
		static ImGuiTableFlags flags =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
			| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit;

		// Update item list if we changed the number of items
		static ImVector<MyItem> items;
		static ImVector<int> selection;
		static bool items_need_sort = false;
		if (items.Size != items_count)
		{
			items.resize(items_count, MyItem());
			for (int n = 0; n < items_count; n++)
			{
				const int template_n = n % IM_ARRAYSIZE(template_items_names);
				MyItem& item = items[n];
				item.ID = n;
				item.Name = template_items_names[template_n];
				item.Quantity = (template_n == 3) ? 10 : (template_n == 4) ? 20 : 0; // Assign default quantities
			}
		}

		const ImDrawList* parent_draw_list = ImGui::GetWindowDrawList();
		const int parent_draw_list_draw_cmd_count = parent_draw_list->CmdBuffer.Size;
		ImVec2 table_scroll_cur, table_scroll_max; // For debug display
		const ImDrawList* table_draw_list = NULL;  // "

		// Submit table
		const float inner_width_to_use = (flags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
		if (ImGui::BeginTable("table_advanced", 6, flags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
		{
			// Declare columns
			// We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
			// This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 0.0f, MyItemColumnID_ID);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
			ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action);
			ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_PreferSortDescending, 0.0f, MyItemColumnID_Quantity);
			ImGui::TableSetupColumn("Description", (flags & ImGuiTableFlags_NoHostExtendX) ? 0 : ImGuiTableColumnFlags_WidthStretch, 0.0f, MyItemColumnID_Description);
			ImGui::TableSetupColumn("Hidden", ImGuiTableColumnFlags_DefaultHide | ImGuiTableColumnFlags_NoSort);
			ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);

			// Sort our data if sort specs have been changed!
			ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs();
			if (sorts_specs && sorts_specs->SpecsDirty)
				items_need_sort = true;
			if (sorts_specs && items_need_sort && items.Size > 1)
			{
				MyItem::s_current_sort_specs = sorts_specs; // Store in variable accessible by the sort function.
				qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
				MyItem::s_current_sort_specs = NULL;
				sorts_specs->SpecsDirty = false;
			}
			items_need_sort = false;

			// Take note of whether we are currently sorting based on the Quantity field,
			// we will use this to trigger sorting when we know the data of this column has been modified.
			const bool sorts_specs_using_quantity = (ImGui::TableGetColumnFlags(3) & ImGuiTableColumnFlags_IsSorted) != 0;

			// Show headers
			ImGui::TableHeadersRow();

			// Show data
			// FIXME-TABLE FIXME-NAV: How we can get decent up/down even though we have the buttons here?
			ImGui::PushButtonRepeat(true);
			#if 1
			// Demonstrate using clipper for large vertical lists
			ImGuiListClipper clipper;
			clipper.Begin(items.Size);
			while (clipper.Step())
			{
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
					#else
			// Without clipper
				{
					for (int row_n = 0; row_n < items.Size; row_n++)
						#endif
					{
						MyItem* item = &items[row_n];
						//if (!filter.PassFilter(item->Name))
						//    continue;

						const bool item_is_selected = selection.contains(item->ID);
						ImGui::PushID(item->ID);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

						// For the demo purpose we can select among different type of items submitted in the first column
						ImGui::TableSetColumnIndex(0);
						char label[32];
						sprintf(label, "%04d", item->ID);
						if (contents_type == CT_Text)
							ImGui::TextUnformatted(label);
						else if (contents_type == CT_Button)
							ImGui::Button(label);
						else if (contents_type == CT_SmallButton)
							ImGui::SmallButton(label);
						else if (contents_type == CT_FillButton)
							ImGui::Button(label, ImVec2(-FLT_MIN, 0.0f));
						else if (contents_type == CT_Selectable || contents_type == CT_SelectableSpanRow)
						{
							ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;
							if (ImGui::Selectable(label, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
							{
								if (ImGui::GetIO().KeyCtrl)
								{
									if (item_is_selected)
										selection.find_erase_unsorted(item->ID);
									else
										selection.push_back(item->ID);
								}
								else
								{
									selection.clear();
									selection.push_back(item->ID);
								}
							}
						}

						if (ImGui::TableSetColumnIndex(1))
							ImGui::TextUnformatted(item->Name);

						// Here we demonstrate marking our data set as needing to be sorted again if we modified a quantity,
						// and we are currently sorting on the column showing the Quantity.
						// To avoid triggering a sort while holding the button, we only trigger it when the button has been released.
						// You will probably need a more advanced system in your code if you want to automatically sort when a specific entry changes.
						if (ImGui::TableSetColumnIndex(2))
						{
							if (ImGui::SmallButton("Chop")) { item->Quantity += 1; }
							if (sorts_specs_using_quantity && ImGui::IsItemDeactivated()) { items_need_sort = true; }
							ImGui::SameLine();
							if (ImGui::SmallButton("Eat")) { item->Quantity -= 1; }
							if (sorts_specs_using_quantity && ImGui::IsItemDeactivated()) { items_need_sort = true; }
						}

						if (ImGui::TableSetColumnIndex(3))
							ImGui::Text("%d", item->Quantity);

						ImGui::TableSetColumnIndex(4);
						ImGui::Text("Lorem");

						if (ImGui::TableSetColumnIndex(5))
							ImGui::Text("1234");

						ImGui::PopID();
					}
			}
			ImGui::PopButtonRepeat();

			// Store some info to display debug details below
			table_scroll_cur = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());
			table_scroll_max = ImVec2(ImGui::GetScrollMaxX(), ImGui::GetScrollMaxY());
			table_draw_list = ImGui::GetWindowDrawList();
			ImGui::EndTable();
		}

		ImGui::End();
	}
}

